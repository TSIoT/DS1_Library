// It must need library
// Library path : E:\Program Files (x86)\Arduino\libraries
#include "SensorLib.h"
#include "DS1_config.h"
#include "DS1.h"
//Add Sensor Library define

#if defined(SENSOR_DHT)
    #include "DHT.h"
    #define DHT_Type DHT22
    DHT dht(DigitalPin,DHT_Type,COUNT);
#endif 

#if defined(SENSOR_SHT2X)
    #include "SHT2x.h"
    SHT2x sht2x;
#endif 

#if defined(SENSOR_PPD42)
    #include "DustSensor.h"
    DustSensor dustsensor;
#endif

#if defined(SENSOR_CO2)
    #include "SoftwareSerialCO2.h"
    SoftwareSerialCO2 co2;
#endif 

 #if defined(SENSOR_TSL2561)
    #include "Digital_Light_TSL2561.h"
    TSL2561_CalculateLux digitial_light;
#endif

 #if defined(SENSOR_HP20X)
    #include "HP20x_dev.h"
    HP20x_dev hp20x;
#endif

#if defined(SENSOR_SUNLIGHT)
   #include "SI114X.h"
    SI114X SI1145 = SI114X();
#endif

#if defined(SENSOR_WEATHER_80422)
   #include "SDL_Weather_80422.h"
   #define pinLED     13 // LED connected to digital pin 13
   #define pinAnem    3  // Anenometer connected to pin 3 - Int 1 - 32u4 / 18 - Int 5 - Mega / Uno pin 2
   #define pinRain    2  // Anenometer connected to pin 2 - Int 0 - 32u4 /  2 - Int 0 - Mega / Uno Pin 3
   #define intAnem    0  // int 0, pin3 for 32u4
   #define intRain    1  // int 1, pin2 for 32u4

   SDL_Weather_80422 sdl_weather_80422(pinAnem, pinRain, intAnem, intRain, AnalogPin, SDL_MODE_INTERNAL_AD);
  
#endif



SensorLib::SensorLib()
{

}
void SensorLib::Init() 
{
#if defined(SENSOR_SHT2X)
    sht2x.begin();
#endif 

#if defined(SENSOR_PPD42)
    dustsensor.init(DigitalPin);
#endif
#if defined(SENSOR_CO2)
    co2.begin();
#endif 

#if defined(SENSOR_TSL2561)
    digitial_light.init();
#endif

 #if defined(SENSOR_HP20X)
    hp20x.begin();
#endif

#if defined(SENSOR_DHT)
    dht.begin(DHT_Type);
#endif 

#if defined(SENSOR_SUNLIGHT)
     while (!SI1145.Begin()) {
        Serial.println("Si1145 is not ready!");
        delay(1000);
     }
#endif


}


#if defined(SENSOR_SHT2X)

float SensorLib::getSHT2xTemperature()
{
    temperatureSHT2x = sht2x.GetTemperature();
    return temperatureSHT2x;
}
float SensorLib::getSHT2xHunidity()
{
    humiditySHT2x = sht2x.GetHumidity();
    return humiditySHT2x;
}
#endif

#if defined(SENSOR_PPD42)
float SensorLib::getPPD42()
{
    PPD42 = dustsensor.readConcentration();
    return PPD42;
}
#endif

#if defined(SENSOR_CO2)
float SensorLib::getCO2()
{
    int i;
    for(i=0;i<30;i++) {
    CO2 = co2.getCO2PPM();
    delay(500);
    }
    return CO2;
}
#endif 

#if defined(SENSOR_TSL2561)
float SensorLib::getLight()
{
    light = digitial_light.readVisibleLux();
    return light;
}
#endif 

#if defined(SENSOR_HP20X)
float SensorLib::getHP20XTemperature()
{
    temperatureHP20X = hp20x.ReadTemperature()/100;
    return temperatureHP20X;
}
float SensorLib::getHP20XPressure()
{
    pressureHP20X = hp20x.ReadPressure()/100;
    return pressureHP20X;
}
float SensorLib::getHP20XAltitude()
{
    altitudeHP20X = hp20x.ReadAltitude()/100;
    return altitudeHP20X;
}
#endif

#if defined(SENSOR_SUNLIGHT)
float SensorLib::getSunLight()
{
    sunlight = (float)SI1145.ReadUV()/100;
    return sunlight;
}
#endif

#if defined(SENSOR_WEATHER_80422)
float SensorLib::getWind()
{
    wind = sdl_weather_80422.current_wind_speed();
    return wind;
}
float SensorLib::getGustWind()
{
    gustwind = sdl_weather_80422.get_wind_gust();
    return gustwind;
}
float SensorLib::getRain()
{
    rain = sdl_weather_80422.get_current_rain_total();
    return rain;
}
int SensorLib::getWindDirection()
{
    winddirection = sdl_weather_80422.current_wind_direction();
    return winddirection;
}
#endif

#if defined(SENSOR_DHT)
float SensorLib::getDHTHunidity()
{
    temperatureDHT = dht.readHumidity();
    return temperatureDHT;
}
float SensorLib::getDHTTemperature()
{
    humidityDHT = dht.readTemperature(false);
    return humidityDHT;
}  
#endif 

#if defined(SENSOR_SOIL_MOISTURE)
float SensorLib::getSoilMoisture()
{
     for(count=0;count<1000;count++) {
        soilmoisture = soilmoisture + analogRead(AnalogPin);
      }
      soilmoisture = soilmoisture/1000;
    return soilmoisture;
}

#endif 

#if defined(SENSOR_BATTERY_POWER)
float SensorLib::getBatteryPower()
{
      for(count=0;count<1000;count++) {
        battery = battery + analogRead(BatteryPin);
      }
      battery = battery/1000;
      return battery;
}

#endif 

