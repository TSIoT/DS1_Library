#include "Arduino.h"
#include "DustSensor.h"

void DustSensor::init(int pin)
{
  starttime = millis();
  _pin = pin;  
}

float DustSensor::readConcentration(void)
{
  duration = pulseIn(_pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) >= sampletime_ms)//if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=&gt;100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    lowpulseoccupancy = 0;
    starttime = millis();
  }
  return concentration;
}
