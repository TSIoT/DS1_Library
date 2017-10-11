#include "SoftwareSerialCO2.h"
#include "DS1.h"
#include <SoftwareSerial.h>
SoftwareSerial s_serial(10, 11);      // TX, RX

void SoftwareSerialCO2::begin(void)
{
  s_serial.begin(9600);
}

bool SoftwareSerialCO2::dataRecieve(void)
{
    byte data[9];
    int i = 0;

    //transmit command data
    for(i=0; i<sizeof(cmd_get_sensor); i++)
    {
        s_serial.write(cmd_get_sensor[i]);
    }
    delay(10);
    //begin reveiceing data
    if(s_serial.available())
    {
        while(s_serial.available())
        {
            for(int i=0;i<9; i++)
            {
                data[i] = s_serial.read();
            }
        }
    }

    for(int j=0; j<9; j++)
    {
        //Serial.print(data[j]);
        //Serial.print(" ");
    }
    //Serial.println("");

    if((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]))) != data[8])
    {
        return false;
    }

    CO2PPM = (int)data[2] * 256 + (int)data[3];
    temperature = (int)data[4] - 40;

    return true;
}
int SoftwareSerialCO2::getTemperature(void)
{
    if(dataRecieve())
      return temperature;
    else 
      return 0;

}
int SoftwareSerialCO2::getCO2PPM(void)
{
    if(dataRecieve())
      return CO2PPM;
    else 
      return 0;
}

