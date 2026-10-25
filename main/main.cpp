#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "Arduino.h"

#include "sensors/sensair_sunrise.hpp"

#define BLINK_GPIO CONFIG_BLINK_GPIO
#define BLINK_PERIOD CONFIG_BLINK_PERIOD

static uint8_t s_led_state = 0;
static led_strip_handle_t led_strip;

static void blink_led(void)
{
    /* If the addressable LED is enabled */
    if (s_led_state) {
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        led_strip_set_pixel(led_strip, 0, 0, 0, 2);
        /* Refresh the strip to send data */
        led_strip_refresh(led_strip);
    } else {
        /* Set all LED off to clear all pixels */
        led_strip_clear(led_strip);
    }
}
static void configure_led(void)
{
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config{};
    strip_config.strip_gpio_num = BLINK_GPIO;
    strip_config.max_leds = 1;
    led_strip_rmt_config_t rmt_config{};
    rmt_config.resolution_hz = 10 * 1000 * 1000; // 10MHz
    rmt_config.flags.with_dma = false;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
}
static void sunrise_task(void*)
{
    SunriseSensor sunrise;
    sunrise.init();

    const TickType_t period =
        pdMS_TO_TICKS(sunrise.measurementPeriodMs());
    TickType_t lastWake = xTaskGetTickCount();

    while (true) {
        xTaskDelayUntil(&lastWake, period);
        sunrise.read();
    }
}
extern "C" void app_main(void) {
	// Enable all arduino functionality
	initArduino();
	
	// Task Creation
    xTaskCreate(sunrise_task, "sunrise", 4096, nullptr, 1, nullptr);
    
    // Configure the LED
    configure_led();
    
    while (1) {
        blink_led();
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(BLINK_PERIOD / portTICK_PERIOD_MS);
        
    }
}
