#include "DS1.h"
#include "SensorLib.h"
#include "Enerlib.h"
#include <avr/sleep.h>
#include <avr/wdt.h>

#define DELAY_TIME 250
#define SLEEPTIMES 2  /*unit : minute*/

DS1 ds1;

SensorLib sensorLib;

Energy energy;

String sendBuffer[2];
byte topicID[]={7,8};
int i;

void setup() {

  /* Clean buffer*/
  for(i=0;i<2;i++) {
    sendBuffer[i].reserve(BUFFER_MAX_SIZE);
  }

  /* Pin power on */
  ds1.EnablePower(DS1::AllPower,true);
  ds1.EnablePower(DS1::RootPower,true);
  ds1.EnablePower(DS1::AnalogPower,true);
  ds1.EnablePower(DS1::I2CPower,true);
  ds1.EnablePower(DS1::DigtialPower,true);

  /* Initial LoRa Moduler */
  ds1.InitRFModule(DS1::RFType_LoRaLan_Slave);

  /* Initial sensor*/
  sensorLib.Init(SensorLib::SensorType_HP20x_Altitude);

  /* LED control */
  ds1.LedFlashing(3);
}

void loop() {
  int i;
  ds1.LedStatus(true);
  
  do{
    sendBuffer[0]=sensorLib.readSensorValue(SensorLib::SensorType_HP20x_Altitude);
    sendBuffer[1]=sensorLib.readSensorValue(SensorLib::SensorType_HP20x_Pressure);
    delay(1000);
  }while((sendBuffer[0].toFloat() == 0.0) || (sendBuffer[1].toFloat() == 0.0));

  Serial.println(sendBuffer[0]);
  Serial.println(sendBuffer[1]);
  
  int times = 2;
  /* LoRa send data : 1 data 2 times */
  ds1.sendDataTimes(1,sendBuffer,topicID,times);
  ds1.EnablePower(DS1::AllPower,false);
  
  ds1.LedStatus(false);

  /* Sleep function */
  ds1.watchdogSleep(SLEEPTIMES); /*unit: minutes*/

  ds1.LedStatus(true);

  /* Wake up and pin power on*/
  ds1.EnablePower(DS1::RootPower,true);
  delay(50);
  ds1.EnablePower(DS1::I2CPower,true);
  ds1.InitRFModule(DS1::RFType_LoRaLan_Slave);
  sensorLib.Init(SensorLib::SensorType_HP20x_Altitude);

}
