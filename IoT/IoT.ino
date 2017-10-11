#include "SensorLib.h"
#include "DS1.h"
#include "DS1_config.h"

#define BUFFER_MAX_SIZE 20
#define DATALENGTH 2
String sendBuffer[DATALENGTH];  
DS1 ds1;
SensorLib lib;
int i;
int ram;
// Define sensor variable
#if defined(SENSOR_SHT2X)
    float sht2x_temperature = 0.0;
    float sht2x_humility = 0.0;
#endif

#if defined(SENSOR_CO2)
    float readCO2 = 0.0;
#endif

#if defined(SENSOR_SOIL_MOISTURE)
    float readSoilmoisture = 0.0;
#endif

#if defined(SENSOR_HP20X)
    float hp20x_temperature = 0.0;
    float hp20x_pressure = 0.0;
    float hp20x_altitude = 0.0;
#endif

#if defined(SENSOR_TSL2561)
    float readlight = 0.0;
#endif

#if defined(SENSOR_SUNLIGHT)
    float readuv = 0.0;
#endif

#if defined(SENSOR_PPD42)
    float readdust = 0.0;
#endif

#if defined(SENSOR_DHT)
    float dht_temperature = 0.0;
    float dht_humility = 0.0;
#endif

#if defined(SENSOR_BATTERY_POWER)
    float battery = 0.0;
#endif

#if defined(RF_S76S)
  #include "S76S.h"
  S76S s76s;
  char *address = "10 0 0";
  char *slaveNode = "10 0 1";
  char *gatewayAddr = "10 0 254";
  char *index = "1";
  bool flag;
  String data = "12345.67890";
#endif
  

#ifdef DEBUG

#endif




byte topicID[]={1,2};

void setup() {
  // Initial Serial port
  Serial.begin(9600);
  Serial1.begin(9600);
  //When terminal open,program continue.
  while(!Serial){};
/*#if defined(SENSOR_DHT)
    Serial.println("SENSOR_DHT");
#endif
#if defined(SENSOR_BATTERY_POWER)
    Serial.println("SENSOR_BATTERY_POWER");
#endif
#if defined(SENSOR_HP20X)
    Serial.println("SENSOR_HP20X");
#endif
#if defined(SENSOR_TSL2561)
    Serial.println("SENSOR_TSL2561");
#endif
#if defined(SENSOR_SHT2X)
    Serial.println("SENSOR_SHT2X");
#endif
#if defined(SENSOR_SOIL_MOISTURE)
    Serial.println("SENSOR_SOIL_MOISTURE");
#endif
#if defined(SENSOR_PPD42)
    Serial.println("SENSOR_PPD42");
#endif
#if defined(SENSOR_CO2)
    Serial.println("SENSOR_CO2");
#endif
#if defined(SENSOR_GUVA_S12D)
    Serial.println("SENSOR_GUVA_S12D");
#endif
#if defined(SENSOR_SUNLIGHT)
    Serial.println("SENSOR_SUNLIGHT");
#endif
#if defined(SENSOR_WEATHER_80422)
    Serial.println("SENSOR_WEATHER_80422");
#endif
  delay(10000);
  */
  // Reserve string buffer 
  for(i=0;i<DATALENGTH;i++) {
    sendBuffer[i].reserve(BUFFER_MAX_SIZE);
  }
  
  // Initial pin power
  ds1.EnablePower(DS1::RootPower,true);
  ds1.EnablePower(DS1::AnalogPower,true);
  ds1.EnablePower(DS1::DigtialPower,true);
  ds1.EnablePower(DS1::I2CPower,true);

  //ds1.InitRFModule(DS1::RFType_LoRaLan_Slave);

  lib.Init();

  ram = freeRam ();
  Serial.print("Setup Ram = ");
  Serial.println(ram);

#if defined(RF_S76S)
  //S76S initial 
  s76s.Init(S76S::RFType_S76S_Slave);

  //S76S set address (Master/Slave mode)
  flag=s76s.SetAddress(address);
  
  //S76S set address (Slave mode)
  flag=s76s.SetGateWayAddress(gatewayAddr);
  
  //S76S Work (true/false)
  flag=s76s.StartWork(true);
#endif
  
}

void loop() {
  
  ds1.LedFlashing(3);

    
  ram = freeRam();
  Serial.print("Ram = ");
  Serial.println(ram);

#if defined(RF_S76S)
  s76s.Publish(data,topicID[1]);
  delay(3000);
#endif
  
// Get Sensor value and print
#if defined(SENSOR_SHT2X)
  sht2x_humility = lib.getSHT2xHunidity();  
  Serial.print("Humility = ");
  Serial.println(sht2x_humility);

  sht2x_temperature = lib.getSHT2xTemperature();
  Serial.print("Temperature = ");
  Serial.println(sht2x_temperature);
#endif

#if defined(SENSOR_CO2)
    readCO2 = lib.getCO2();
    Serial.print("CO2 = ");
    Serial.println(readCO2);
#endif

#if defined(SENSOR_SOIL_MOISTURE)
    readSoilmoisture = lib.getSoilMoisture();
    Serial.print("SoilMoisture = ");
    Serial.println(readSoilmoisture); 
#endif

#if defined(SENSOR_HP20X)  
    hp20x_temperature = lib.getHP20XTemperature();
    Serial.print("HP20X temperature = ");
    Serial.println(hp20x_temperature);
    
    hp20x_pressure = lib.getHP20XPressure();
    Serial.print("HP20X pressure = ");
    Serial.println(hp20x_pressure);
    
    hp20x_altitude = lib.getHP20XAltitude();
    Serial.print("HP20X altitude = ");
    Serial.println(hp20x_altitude);
#endif

#if defined(SENSOR_TSL2561)
    readlight = lib.getLight();
    Serial.print("Light = ");
    Serial.println(readlight); 
#endif

#if defined(SENSOR_SUNLIGHT)
    readuv = lib.getSunLight();
    Serial.print("Sunlight = ");
    Serial.println(readuv); 
#endif

#if defined(SENSOR_PPD42)
    readdust = lib.getPPD42();
    Serial.print("Dust = ");
    Serial.println(readdust);
#endif

#if defined(SENSOR_DHT)
    dht_temperature = lib.getDHTTemperature();
    dht_humility = lib.getDHTHunidity();

    Serial.print("DHT_Temperature = ");
    Serial.println(dht_temperature);
    
    Serial.print("DHT_Hunidity = ");
    Serial.println(dht_humility);
#endif

#if defined(SENSOR_BATTERY_POWER)
    battery = lib.getBatteryPower();
    Serial.print("Battery = ");
    Serial.println(battery);
#endif


// Sleep mode ,close power,wake up,open power
#if defined(SLEEP_MODE)
  delay(3000);
  ds1.EnablePower(DS1::RootPower,false);
  ds1.EnablePower(DS1::AnalogPower,false);
  ds1.EnablePower(DS1::DigtialPower,false);
  ds1.EnablePower(DS1::I2CPower,false);
  
  delay(250);
  ds1.watchdogSleep(60);
  delay(250);
  
  ds1.EnablePower(DS1::RootPower,true);
  ds1.EnablePower(DS1::AnalogPower,true);
  ds1.EnablePower(DS1::DigtialPower,true);
  ds1.EnablePower(DS1::I2CPower,true);
  
  lib.Init();

  ds1.InitRFModule(DS1::RFType_LoRaLan_Slave);
#endif


}


int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

