#ifndef SensorLib_h
#define SensorLib_h
#include <Arduino.h>
#include "DS1_config.h"
#include "Wire.h"

class SensorLib
{
    private:
    
#if defined(SENSOR_SHT2X)
    float temperatureSHT2x;
    float humiditySHT2x;
#endif
#if defined(SENSOR_PPD42)
    float PPD42;
#endif

#if defined(SENSOR_CO2)
    float CO2;
#endif

#if defined(SENSOR_TSL2561)
    float light;
#endif
#if defined(SENSOR_HP20X)
    float temperatureHP20X;
    float pressureHP20X;
    float altitudeHP20X;
#endif
#if defined(SENSOR_SUNLIGHT)
    float sunlight;
#endif

#if defined(SENSOR_WEATHER_80422)
    float wind;
    float gustwind;
    float winddirection;
    float rain;
#endif

#if defined(SENSOR_DHT)
    float temperatureDHT;
    float humidityDHT;
#endif

#if defined(SENSOR_SOIL_MOISTURE)
    float soilmoisture;
#endif

#if defined(SENSOR_BATTERY_POWER)
    float battery;
#endif

#if defined(SENSOR_BATTERY_POWER) || defined(SENSOR_SOIL_MOISTURE)
    int count;
#endif

    public:
    SensorLib();
    void Init();
    
#if defined(SENSOR_SHT2X)
    float getSHT2xTemperature();
    float getSHT2xHunidity();
#endif

#if defined(SENSOR_PPD42)
    float getPPD42();
#endif
#if defined(SENSOR_CO2)
    float getCO2();
#endif

#if defined(SENSOR_TSL2561)
    float getLight();
#endif

#if defined(SENSOR_HP20X)
    float getHP20XTemperature();
    float getHP20XPressure();
    float getHP20XAltitude();
#endif

#if defined(SENSOR_SUNLIGHT)
    float getSunLight();
#endif

#if defined(SENSOR_WEATHER_80422)
    float getWind();
    float getGustWind();
    float getRain();
    int getWindDirection(); 
#endif

#if defined(SENSOR_DHT)
    float getDHTTemperature();
    float getDHTHunidity();
#endif 

#if defined(SENSOR_SOIL_MOISTURE)
    float getSoilMoisture();
#endif 

#if defined(SENSOR_BATTERY_POWER)
    float getBatteryPower();
#endif 

};


#endif
