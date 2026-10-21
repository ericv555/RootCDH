#pragma once
#include "senseair_sunrise.h"

class SunriseSensor {
public:
	void read();
    void init();
    void send_to_comms();
};
