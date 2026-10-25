#pragma once
#include "senseair_sunrise.h"
#include "sensor.hpp"

class SunriseSensor : public Sensor{
public:
	SunriseSensor();
	void read() override;
    void init() override;
    void send_to_comms() override;
    void printErrorStatus(uint16_t errorStatus);
	uint32_t measurementPeriodMs() const;
private:
    SunriseError error;
    SunriseI2C sensor;
};
