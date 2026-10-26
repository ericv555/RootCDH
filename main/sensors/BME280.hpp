#pragma once
#include "Adafruit_BME280.h"
#include "sensor.hpp"

class Bme280Sensor : public Sensor {
public:
	Bme280Sensor();
	void read() override;
	void init() override;
	void send_to_comms() override;
	uint32_t measurementPeriodMs() const;

private:
	Adafruit_BME280 sensor;
	bool initialized = false;
	float temperatureC = 0.0f;
	float pressureHpa = 0.0f;
	float humidityPct = 0.0f;
	float altitudeM = 0.0f;
};
