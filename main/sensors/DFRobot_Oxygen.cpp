#include "DFRobot_Oxygen.hpp"
#include "esp_log.h"
#include <cstdint>

static constexpr char TAG[] = "oxygen";

/* i2c slave address — dial A0/A1: ADDRESS_0..ADDRESS_3 (default ADDRESS_3) */
constexpr uint8_t OXYGEN_IIC_ADDRESS = ADDRESS_3;

/* Samples averaged per reading (library range 1–100) */
constexpr uint8_t COLLECT_NUMBER = 10;

/* How often the FreeRTOS task should call read() */
constexpr uint32_t MEASUREMENT_PERIOD_MS = 1000u;

DfRobotOxygenSensor::DfRobotOxygenSensor() : sensor() {}

uint32_t DfRobotOxygenSensor::measurementPeriodMs() const {
	return MEASUREMENT_PERIOD_MS;
}

void DfRobotOxygenSensor::init() {
	if (!sensor.begin(OXYGEN_IIC_ADDRESS)) {
		ESP_LOGE(TAG, "I2C device error (addr 0x%02X) — check wiring / address switch",
				 static_cast<unsigned>(OXYGEN_IIC_ADDRESS));
		initialized = false;
		return;
	}

	initialized = true;
	ESP_LOGI(TAG, "I2C connect success (addr 0x%02X), period=%lu ms",
			 static_cast<unsigned>(OXYGEN_IIC_ADDRESS),
			 static_cast<unsigned long>(MEASUREMENT_PERIOD_MS));
}

void DfRobotOxygenSensor::read() {
	if (!initialized) {
		return;
	}

	oxygenPctVol = sensor.getOxygenData(COLLECT_NUMBER);
	ESP_LOGI(TAG, "oxygen concentration is %.2f %%vol", oxygenPctVol);
}

void DfRobotOxygenSensor::send_to_comms() {
	// Stub — wire into the shared comms path when that exists
}
