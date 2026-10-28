#pragma once
#include "DFRobot_OxygenSensor.h"
#include "sensor.hpp"

class DfRobotOxygenSensor : public Sensor {
public:
	DfRobotOxygenSensor();
	void read() override;
	void init() override;
	void send_to_comms() override;
	uint32_t measurementPeriodMs() const;

private:
	DFRobot_OxygenSensor sensor;
	bool initialized = false;
	float oxygenPctVol = 0.0f;
};
