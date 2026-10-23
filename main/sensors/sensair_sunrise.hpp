#pragma once
#include "senseair_sunrise.h"
#include "sensor.hpp"

class SunriseSensor : public Sensor{
public:
	void read() override;
    void init() override;
    void send_to_comms() override;
};
