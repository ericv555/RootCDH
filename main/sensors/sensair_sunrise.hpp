#pragma once
#include "senseair_sunrise.h"
#include "sensor.hpp"
#include "Wire.h"

class SunriseSensor : public Sensor{
public:
	SunriseSensor(TwoWire &bus, uint8_t address, int enablePin, int nrdyPin,
                  int comselPin);
	void read() override;
    void init() override;
    void send_to_comms() override;
    void printErrorStatus(uint16_t errorStatus);
	uint32_t measurementPeriodMs() const;
private:
	SunriseError error;
	int comselPin_;
    SunriseI2C sensor;
};
