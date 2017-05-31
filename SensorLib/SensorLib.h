#ifndef SensorLib_h
#define SensorLib_h
#include <Arduino.h>

#define BUFFER_MAX_SIZE 20
#define FLOAT_ROUND 2
#define DATA_LEN 2

class SensorLib
{
public:
  enum SensorLib_Type
  {
    SensorType_Moisture,
    SensorType_AirQuality,
    SensorType_DHT11_Humidity,
    SensorType_DHT11_Temperature,
    SensorType_DHT22_Humidity,
    SensorType_DHT22_Temperature,
    SensorType_DigitalLight,
    SensorType_SHT2X_Humidity,
    SensorType_SHT2X_Temperature,
    SensorType_Weather_WindGust,
    SensorType_Weather_WindSpeed,
    SensorType_Weather_WindDirection,
    SensorType_Weather_Rain,
    SensorType_HP20x_Altitude,
    SensorType_HP20x_Pressure,
    SensorType_HP20x_Temperature,
    SensorType_DustSensor
  };

  SensorLib();
  
  void Init(SensorLib_Type);
  String readSensorValue(SensorLib_Type);
  int freeRam(void);
  

private:
  String sensorBuffer = "";
  String testBuffer[DATA_LEN];
  
};

#endif
