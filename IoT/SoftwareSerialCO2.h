#ifndef SoftwareSerialCO2_h
#define SoftwareSerialCO2_h
#include <Arduino.h>


class SoftwareSerialCO2
{
public:
  void begin(void);
	int getTemperature();
  int getCO2PPM();
  int temperature;
  int CO2PPM;


private:
  
	const unsigned char cmd_get_sensor[9] =
  {
    0xff, 0x01, 0x86, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x79
  };

  const unsigned char cmd_calibrate[9] = 
  {
    0xff, 0x87, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf2
  };

  unsigned char dataRevice[9];
  bool dataRecieve();
};

#endif
