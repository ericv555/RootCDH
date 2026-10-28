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
    int16_t get_CO2();
    float get_temp();
private:
	SunriseError error;
	int comselPin_;
    SunriseI2C sensor;
    /// CO2 is stored in ppm
    int16_t CO2_reading;
    /// Temp of sensor internals held in deg C
    float temp;
};
