#include "BME280.hpp"
#include "esp_log.h"
#include <Wire.h>
#include <cstdint>

static constexpr char TAG[] = "bme280";

constexpr int I2C_SDA = 21;
constexpr int I2C_SCL = 22;

// Sea-level pressure used for approximate altitude (hPa)
constexpr float SEALEVELPRESSURE_HPA = 1013.25f;

constexpr uint32_t MEASUREMENT_PERIOD_MS = 1000u;

Bme280Sensor::Bme280Sensor() : sensor() {}

uint32_t Bme280Sensor::measurementPeriodMs() const {
	return MEASUREMENT_PERIOD_MS;
}

void Bme280Sensor::init() {
	Wire.begin(I2C_SDA, I2C_SCL);

	bool found = sensor.begin(BME280_ADDRESS_ALTERNATE, &Wire);
	if (!found) {
		found = sensor.begin(BME280_ADDRESS, &Wire);
	}

	if (!found) {
		ESP_LOGE(TAG,
				 "Could not find a valid BME280 (check wiring/address). "
				 "SensorID was: 0x%02lX "
				 "(0xFF bad address; 0x56-0x58 BMP280; 0x60 BME280; 0x61 BME680)",
				 static_cast<unsigned long>(sensor.sensorID()));
		initialized = false;
		return;
	}

	initialized = true;
	ESP_LOGI(TAG, "BME280 found (SensorID 0x%02lX), period=%lu ms",
			 static_cast<unsigned long>(sensor.sensorID()),
			 static_cast<unsigned long>(MEASUREMENT_PERIOD_MS));
}

void Bme280Sensor::read() {
	if (!initialized) {
		return;
	}

	temperatureC = sensor.readTemperature();
	pressureHpa = sensor.readPressure() / 100.0f;
	humidityPct = sensor.readHumidity();
	altitudeM = sensor.readAltitude(SEALEVELPRESSURE_HPA);

	ESP_LOGI(TAG, "T=%.2f C  P=%.2f hPa  H=%.2f %%  alt=%.2f m",
			 temperatureC, pressureHpa, humidityPct, altitudeM);
}

void Bme280Sensor::send_to_comms() {
	// fill this jawn out when we got comms code. 
}
