#ifndef LoRaBee_H_INCLUDED
#define LoRaBee_H_INCLUDED
#include <Arduino.h>


#define LoRa_MAX_BUFFER_SIZE 33

#define MQTT_HEADER_SIZE 3
#define PublishType 0x10

#define RootPowerEnablePin A1
#define AnalogPowerEnablePin 4
#define I2CPowerEnablePin 5
#define DigitalPowerEnablePin 18
#define RelayPin 21
#define AnalogPin 8
#define DigitalPin 10
#define LED A2

class DS1
{
public:
	enum RFType
	{
		RFType_Unknown,
		RFType_LoRaLan_Slave,
		RFType_LoRaLan_Master
	};

	enum PowerType
	{
		AllPower,
		RootPower,
		AnalogPower,
		I2CPower,
		DigtialPower,
	};

	DS1();

	void InitRFModule(RFType type);

	void EnablePower(PowerType powerType, bool enable);

	void SendDataToMaster(char *buffer,int dataLength);
	
	void GetSelfLoraAddress(char *buffer,int bufferLength);

	bool DS1::JoinNode(char *nodeAddress);

	void Publish(String sendPayload, byte topicId);

	//void MultiPublish(int publishCount, byte **sendPayload, int *len, byte *topicId);
	void MultiPublish(int publishCount, String sendPayload[], byte *topicId);
  void watchdogSleep(int minutes);
  void sendDataTimes(int PublishCount ,String sendBuffer[],byte *topicID ,int times);
  void LedStatus(boolean power);
  void LedFlashing(int flashingTimes);

private:	
	byte sendBuffer[LoRa_MAX_BUFFER_SIZE] = { 0 };//
	char *cmdSendData="LoraNodeData";	
	char *cmdGetAddress="LoraGetMyAddr";
	char endSymbol='\r';


	RFType currentRfType = RFType_Unknown;

	bool SendAtCommand(char *cmd);
	int readDataFromSerial(char *buffer,unsigned long timeout);	
	void resetRecv();
  void myWatchdogEnable(const byte);
  int handleDevData();
	

};






#endif
