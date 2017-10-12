#ifndef DustSensor_H_INCLUDED
#define DustSensor_H_INCLUDED
#include"Arduino.h"

class DustSensor
{
public:
	float readConcentration(void);
	void init(int pin);
private:
	int _pin;
	unsigned long duration;
	unsigned long starttime;
	unsigned long sampletime_ms = 30000; //sampe 30s&nbsp;;
	unsigned long lowpulseoccupancy = 0;
	float ratio = 0;
	float concentration = 0;
};

#endif
