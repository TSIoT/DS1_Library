#ifndef S76S_H_INCLUDED
#define S76S_H_INCLUDED
#include <Arduino.h>
#define LoRa_MAX_BUFFER_SIZE 33

#define MQTT_HEADER_SIZE 3
#define PublishType 0x10
#define BaudRate 9600

class S76S
{
public:
  enum RFType
  {
    RFType_Unknown,
    RFType_S76S_Slave,
    RFType_S76S_Master
  };

  enum MACType
  {
    MACType_Unknown,
    MACType_MyAddress,
    MACType_GatewayAddress
  };


	S76S();

	void Init(RFType type);

	bool SetAddress(char *address);

	bool SetGateWayAddress(char *address);
	
	bool StartWork(bool work);
/*
	bool AddSlaveNode(char *address,char *index);

	

  bool SaveSlaveNode(char *index);

  bool RemoveSlaveNode(char *index);

  void MasterDownlink(char *address,char *data);
*/
  void SlaveUplink(char *buffer,int dataLength);

	//void MultiPublish(int publishCount, byte **sendPayload, int *len, byte *topicId);
  void Publish(String sendPayload, byte topicId);
	void MultiPublish(int publishCount, String sendPayload[], byte *topicId);
  void sendDataTimes(int PublishCount ,String sendBuffer[],byte *topicID ,int times);
  String GetSerialData(void);

private:	
	byte sendBuffer[LoRa_MAX_BUFFER_SIZE] = { 0 };//
	char cmdEndSymbol='\r';

	RFType currentRfType = RFType_Unknown;

	bool SendCommand(char *cmd);
	int readDataFromSerial(char *buffer,unsigned long timeout);	
	void resetRecv();
  int handleDevData();
  String ack;
	

};






#endif
