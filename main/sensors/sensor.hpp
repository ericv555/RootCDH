#pragma once
class Sensor {
public:
	Sensor() = default;
	virtual ~Sensor() = default;`
	virtual void init() = 0;
	virtual void read() = 0;
	virtual void send_to_comms() = 0;
};
