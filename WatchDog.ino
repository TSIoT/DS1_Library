#include "DS1.h"
#include "SensorLib.h"
#include "Enerlib.h"
#include <avr/sleep.h>
#include <avr/wdt.h>

#define DELAY_TIME 250
#define SLEEPTIMES 2


DS1 ds1;

SensorLib sensorLib;

Energy energy;

String sendBuffer[2];
byte topicID[]={7,8};
//String sendBuffer;
int i;

void setup() {

  for(i=0;i<2;i++) {
    sendBuffer[i].reserve(BUFFER_MAX_SIZE);
  }
  //sendBuffer.reserve(BUFFER_MAX_SIZE);
  //ds1.EnablePower(DS1::AllPower,true);
  ds1.EnablePower(DS1::RootPower,true);
  //ds1.EnablePower(DS1::AnalogPower,true);
  ds1.EnablePower(DS1::I2CPower,true);
  //ds1.EnablePower(DS1::DigtialPower,true);
  ds1.InitRFModule(DS1::RFType_LoRaLan_Slave);

  sensorLib.Init(SensorLib::SensorType_HP20x_Altitude);
  ds1.LedFlashing(3);
}

void loop() {
  int i;
  delay(3000);
  ds1.LedStatus(true);

  while(!sendBuffer[0].compareTo("0.0")) {
    sendBuffer[0]=sensorLib.readSensorValue(SensorLib::SensorType_HP20x_Altitude);
    sendBuffer[1]=sensorLib.readSensorValue(SensorLib::SensorType_HP20x_Pressure);
    delay(1000);
  }
  
  int times = 10;
  ds1.sendDataTimes(sendBuffer,times,topicID);
  ds1.EnablePower(DS1::AllPower,false);
  
  ds1.LedStatus(false);
  
  //ds1.watchdogSleep(SLEEPTIMES); /*unit: minutes*/

  ds1.LedStatus(true);
  
  ds1.EnablePower(DS1::RootPower,true);
  delay(50);
  ds1.EnablePower(DS1::I2CPower,true);
  ds1.InitRFModule(DS1::RFType_LoRaLan_Slave);
  sensorLib.Init(SensorLib::SensorType_HP20x_Altitude);
  
  ds1.LedStatus(false);
  

}