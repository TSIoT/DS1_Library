#include "SensorLib.h"
#include "DHT.h"
#include "DS1.h"
#include "AirQuality.h"
#include "Digital_Light_TSL2561.h"
#include "SHT2x.h"
#include "SDL_Weather_80422.h"
#include "HP20x_dev.h"
#include "KalmanFilter.h"
#include "DustSensor.h"

#define pinLED     13 // LED connected to digital pin 13
#define pinAnem    3  // Anenometer connected to pin 3 - Int 1 - 32u4 / 18 - Int 5 - Mega / Uno pin 2
#define pinRain    2  // Anenometer connected to pin 2 - Int 0 - 32u4 /  2 - Int 0 - Mega / Uno Pin 3
#define intAnem    0  // int 0, pin3 for 32u4
#define intRain    1  // int 1, pin2 for 32u4

DHT dht(DigitalPin,DHT11,COUNT);
AirQuality airquality;
TSL2561_CalculateLux digitial_light;
SHT2xClass sht2x;
SDL_Weather_80422 sdl_weather_80422(pinAnem, pinRain, intAnem, intRain, AnalogPin, SDL_MODE_INTERNAL_AD);
HP20x_dev hp20x;
DustSensor dustsensor;

SensorLib::SensorLib()
{
    sensorBuffer.reserve(BUFFER_MAX_SIZE);
}

void SensorLib::Init(SensorLib_Type sensorType) 
{
   
  switch(sensorType)
  {
      case SensorType_Moisture:
      case SensorType_AirQuality:
      case SensorType_UV:
          sensorBuffer ="";
          pinMode(AnalogPowerEnablePin, OUTPUT);
          digitalWrite(AnalogPowerEnablePin, true);
          break;
      case SensorType_DHT11_Humidity:
      case SensorType_DHT11_Temperature:
          sensorBuffer ="";
          pinMode(DigitalPowerEnablePin, OUTPUT);
          digitalWrite(DigitalPowerEnablePin, true);
          dht.begin(DHT11);
          break;
      case SensorType_DHT22_Humidity:
      case SensorType_DHT22_Temperature:
          sensorBuffer ="";
          pinMode(DigitalPowerEnablePin, OUTPUT);
          digitalWrite(DigitalPowerEnablePin, true);
          dht.begin(DHT22);
          break;
      case SensorType_DigitalLight:
          sensorBuffer ="";
          pinMode(I2CPowerEnablePin, OUTPUT);
          digitalWrite(I2CPowerEnablePin, true);
          digitial_light.init();
          break;
      case SensorType_SHT2X_Humidity:
      case SensorType_SHT2X_Temperature:
          sensorBuffer ="";
          pinMode(I2CPowerEnablePin, OUTPUT);
          digitalWrite(I2CPowerEnablePin, true);
          break;
       case SensorType_Weather_WindGust:
       case SensorType_Weather_WindSpeed:
       case SensorType_Weather_WindDirection:
       case SensorType_Weather_Rain:
          sensorBuffer ="";
          pinMode(AnalogPowerEnablePin, OUTPUT);
          digitalWrite(AnalogPowerEnablePin, true);
          pinMode(I2CPowerEnablePin, OUTPUT);
          digitalWrite(I2CPowerEnablePin, true);
          break; 
       case SensorType_HP20x_Altitude:
       case SensorType_HP20x_Pressure:
       case SensorType_HP20x_Temperature:
          sensorBuffer ="";
          pinMode(I2CPowerEnablePin, OUTPUT);
          digitalWrite(I2CPowerEnablePin, true);
          hp20x.begin();
          break;
       case SensorType_DustSensor:
          sensorBuffer ="";
          pinMode(DigitalPowerEnablePin, OUTPUT);
          digitalWrite(DigitalPowerEnablePin, true);
          dustsensor.init(DigitalPin);
          break;   
  }
}

String SensorLib::readSensorValue(SensorLib_Type sensorType)
{
  switch(sensorType)
  {
    case SensorType_Moisture:
    case SensorType_AirQuality:
      sensorValue = 0;
      for(count=0;count<1000;count++) {
        sensorValue = sensorValue + analogRead(AnalogPin);
      }
      sensorValue = sensorValue/1000;
      sensorBuffer = String(sensorValue,FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_UV:
      sensorBuffer = UVSensor();
      return sensorBuffer;      
    case SensorType_DHT11_Humidity:
    case SensorType_DHT22_Humidity:
      sensorBuffer = String(dht.readHumidity(),FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_DHT11_Temperature:
    case SensorType_DHT22_Temperature:
      sensorBuffer = String(dht.readTemperature(false),FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_DigitalLight:
      sensorBuffer = String(digitial_light.readVisibleLux(),DEC);
      return sensorBuffer;
    case SensorType_SHT2X_Humidity:
      sensorBuffer = String(sht2x.GetHumidity(),FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_SHT2X_Temperature:
      sensorBuffer = String(sht2x.GetTemperature(),FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_Weather_WindGust:
      sensorBuffer = String(sdl_weather_80422.get_wind_gust(),FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_Weather_WindSpeed:
      sensorBuffer = String(sdl_weather_80422.current_wind_speed(),FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_Weather_WindDirection:
      sensorBuffer = String(sdl_weather_80422.current_wind_direction(),FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_Weather_Rain:
      sensorBuffer = String(sdl_weather_80422.get_current_rain_total(),FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_HP20x_Altitude:
      sensorBuffer = String(hp20x.ReadAltitude()/100.0,FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_HP20x_Pressure:
      sensorBuffer = String(hp20x.ReadPressure()/100.0,FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_HP20x_Temperature:
      sensorBuffer = String(hp20x.ReadTemperature()/100.0,FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_DustSensor:
      sensorBuffer = String(dustsensor.readConcentration(),FLOAT_ROUND);
      return sensorBuffer;
    case SensorType_Battery:
      sensorValue = 0;
      for(count=0;count<1000;count++) {
        sensorValue = sensorValue + analogRead(BatteryPin);
      }
      sensorValue = sensorValue/1000;
      sensorBuffer = String(sensorValue,FLOAT_ROUND);
      return sensorBuffer;
  }
}
int SensorLib::freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

String SensorLib::UVSensor(void)
{
  float Vsig;
  
  int sensorValue;
  long  sum=0;
  for(int i=0;i<1024;i++)
   {  
      sensorValue=analogRead(AnalogPin);
      sum=sensorValue+sum;
      delay(2);
   }   
 sum = sum >> 10;
 Vsig = sum*4980.0/1023.0; // Vsig is the value of voltage measured from the SIG pin of the Grove interface
 sensorBuffer = String(Vsig,FLOAT_ROUND);
 return sensorBuffer;
}

