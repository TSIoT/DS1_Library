#include <avr/sleep.h>
#include <avr/wdt.h>
#include "DS1.h"

// watchdog interrupt
ISR(WDT_vect) 
{
  wdt_disable();  // disable watchdog
}

//public
DS1::DS1()
{	
	
}


void DS1::InitRFModule(RFType type)
{
	this->currentRfType = type;
	int cmdIndex = 0;
	char *cmd[] = { "LoraSystemMode inNormal\r","LoraMode SLAVE\r","LoraMode MASTER\r" };

	switch (type)
	{
	case RFType_LoRaLan_Slave:
		
		
		Serial.begin(9600);
		Serial1.begin(9600);

		Serial.print("LoraSystemMode inNormal");

		while (!SendAtCommand(cmd[cmdIndex])) { delay(1000); }

		Serial.print("LoraSystemMode Slave");
		cmdIndex = 1;

		while (!SendAtCommand(cmd[cmdIndex])) { delay(1000); }

		
		break;
	case RFType_LoRaLan_Master:


		Serial.begin(9600);
		Serial1.begin(9600);

		Serial.print("LoraSystemMode inNormal");

		while (!SendAtCommand(cmd[cmdIndex])) { delay(1000); }


		Serial.print("LoraSystemMode Master");
		cmdIndex = 2;

		while (!SendAtCommand(cmd[cmdIndex])) { delay(1000); }


		break;
	
	}

	Serial.println("RF module init finish");
	
}

//Power control



void DS1::EnablePower(PowerType powerType, bool enable)
{
	int targetPin = 0;

	switch (powerType)
	{
	case AllPower:

		break;
	case RootPower:
		targetPin = RootPowerEnablePin;
		break;
	case AnalogPower:
		targetPin = AnalogPowerEnablePin;
		break;
	case I2CPower:
		targetPin = I2CPowerEnablePin;
		break;
	case DigtialPower:
		targetPin = DigitalPowerEnablePin;
		break;
	}

	if (powerType== AllPower)
	{
		Serial.println("All power enable");
		pinMode(RootPowerEnablePin, OUTPUT);
		digitalWrite(RootPowerEnablePin, enable);
		pinMode(AnalogPowerEnablePin, OUTPUT);
		digitalWrite(AnalogPowerEnablePin, enable);
		pinMode(I2CPowerEnablePin, OUTPUT);
		digitalWrite(I2CPowerEnablePin, enable);
		pinMode(DigitalPowerEnablePin, OUTPUT);
		digitalWrite(DigitalPowerEnablePin, enable);
	}
	else
	{
		pinMode(targetPin, OUTPUT);		
		digitalWrite(targetPin, enable);
	}
	
}



//LoRa
void DS1::SendDataToMaster(char *buffer,int dataLength)
{
	//int sendLength=0;
	//char codeBuffer[MAX_PACKAGE_SIZE]={0};
	//sendLength=Base64encode(codeBuffer, buffer, dataLength);
	switch (this->currentRfType)
	{
	case RFType_LoRaLan_Slave:
		Serial1.write(cmdSendData, strlen(cmdSendData));
		Serial1.write(' ');
		Serial1.write(buffer, dataLength);
		Serial1.write(endSymbol);
    //Serial.println("Data send");
		this->resetRecv();
		break;
	}
	
}

void DS1::GetSelfLoraAddress(char *buffer,int bufferLength)
{
	char *cmd="LoraGetMyAddr";
	
	memset(buffer,0,bufferLength);

	Serial1.write(cmd,strlen(cmd));
	Serial1.write(endSymbol);
	readDataFromSerial(buffer,100);		
}

bool DS1::SendAtCommand(char *cmd)
{
	bool succeed = 0;
	int recvLen = 0;
	const int maxBufSize = 20;
	char recvBuf[maxBufSize] = { 0 };

	Serial1.write(cmd, strlen(cmd));
	memset(recvBuf, 0, maxBufSize);
	recvLen = this->readDataFromSerial(recvBuf, 100);
	if (recvLen == 2 && strcmp("OK", recvBuf) == 0)
	{
		succeed = 1;
		Serial.println("[OK]");
	}
	else
	{
		succeed = 0;
		Serial.println("[Failed]");
	}
	return succeed;
}

bool DS1::JoinNode(char *address)
{
	bool isSucceed = 0;
	char *cmd = "LoraJoinNode ";
	char cmdEndSymbol = '\r';
	int recvLen = 0;
	const int maxBufSize = 20;
	char recvBuf[maxBufSize] = { 0 };

	Serial.print("Add node:[");
	Serial.print(address);
	Serial.println("]");

	for (int i = 0; i <1; i++)
	{
		Serial1.write(cmd, strlen(cmd));
		Serial1.write(address, strlen(address));
		Serial1.write(cmdEndSymbol);

		recvLen = this->readDataFromSerial(recvBuf, 100);
		if (recvLen == 2 && strcmp("OK", recvBuf) == 0)
		{
			Serial.println("Command OK!");						
		}

		//recvLen = this->readDataFromSerial(recvBuf, this->loraResponseWaitTimeout);
		recvLen = this->readDataFromSerial(recvBuf, 100);
		if (recvLen == 6 && strcmp("JoinOK", recvBuf) == 0)
		{
			Serial.print(address);
			Serial.println(" Join OK!");
			//cout << "Ready to send data to[" << address << "]" << endl;
			isSucceed = 1;
			break;
		}
	}

	return isSucceed;
}



//MQTT
void DS1::Publish(String sendPayload, byte topicId)
{
  	this->sendBuffer[0] = (byte)sendPayload.length() + MQTT_HEADER_SIZE;
 	this->sendBuffer[1] = (byte)PublishType;
  	this->sendBuffer[2] = topicId; //topic id
  
  for (int i = 0; i<sendPayload.length(); i++)
  {
    	this->sendBuffer[i + MQTT_HEADER_SIZE] = sendPayload.charAt(i);
  }
 
 	 this->SendDataToMaster(this->sendBuffer, sendPayload.length() + MQTT_HEADER_SIZE);
}

void DS1::MultiPublish(int publishCount, String sendPayload[], byte *topicId)
{
	int index = 0;
	for (int i = 0; i<publishCount; i++)
	{
		this->sendBuffer[index++] = (byte)sendPayload[i].length() + MQTT_HEADER_SIZE;
		Serial.print(this->sendBuffer[index - 1]);
		Serial.print(" ");

		this->sendBuffer[index++] = (byte)PublishType;
		Serial.print(this->sendBuffer[index - 1]);
		Serial.print(" ");

		this->sendBuffer[index++] = topicId[i]; //topic id
		Serial.print(this->sendBuffer[index - 1]);
		Serial.print(" ");


		for (int j = 0; j<sendPayload[i].length() ; j++)
		{
			this->sendBuffer[index++] = sendPayload[i].charAt(j);
			Serial.print(this->sendBuffer[index - 1]);
			Serial.print(" ");
		}
	}
	Serial.println();
	this->SendDataToMaster(this->sendBuffer, index);
}
void DS1::sendDataTimes(int PublishCount ,String sendBuffer[],byte *topicID ,int times) 
{
  char tempByte;
    for(int i=0;i<times;i++)
    {
       while(handleDevData() == 0) {
          ;
       }
       MultiPublish(PublishCount,sendBuffer,topicID);
       
       //resetRecv();
    }
}

void DS1::watchdogSleep(int minutes)
{
   /*********************
   * SLEEP BIT PATTERNS
   * 1 second:  0b000110
   * 2 seconds: 0b000111
   * 4 seconds: 0b100000
   * 8 seconds: 0b100001
   *********************/
   for(int i=0;i<minutes;i++) /*60 = 8x7 + 4 */
   { 
      myWatchdogEnable (0b100001);
      myWatchdogEnable (0b100001);
      myWatchdogEnable (0b100001);
      myWatchdogEnable (0b100001);
      myWatchdogEnable (0b100001);
      myWatchdogEnable (0b100001);
      myWatchdogEnable (0b100001);
      myWatchdogEnable (0b100000);   
   }
}
void DS1::LedStatus(boolean power)
{
    pinMode(LED, OUTPUT);

    if(power)
      digitalWrite(LED, HIGH);
    else
      digitalWrite(LED, LOW);
}

void DS1::LedFlashing(int flashingTimes)
{
    pinMode(LED, OUTPUT);
    
    for (int k = 1; k <= flashingTimes; k = k + 1) {
        digitalWrite(LED, HIGH);
        delay(250L);
        digitalWrite(LED, LOW);
        delay(250L);
    }
}
String DS1::GetSerialData(void)
{
  int len;
  String data="";
  delay(100);
  len = Serial1.available();
  if (len>0)
  {
    for (int i = 0; i < len; i++)
    {     
      data = data + (char)Serial1.read();
      //Serial.print(Serial1.read());
    }
    //Serial.println("--------------");
  }
  return data;
}
/*
void DS1::MultiPublish(int publishCount, byte **sendPayload, int *len, byte *topicId)
{
	int index = 0;
	for (int i = 0; i<publishCount; i++)
	{
		this->sendBuffer[index++] = (byte)len[i] + MQTT_HEADER_SIZE;
		this->sendBuffer[index++] = (byte)PublishType;
		this->sendBuffer[index++] = topicId[i]; //topic id
		for (int j = 0; j<len[i]; j++)
		{
			this->sendBuffer[index++] = sendPayload[i][j];
			Serial.print(this->sendBuffer[index - 1]);
			Serial.print(" ");
		}
	}
	Serial.println();
	this->SendDataToMaster(this->sendBuffer, index);
}
*/

//private


int DS1::readDataFromSerial(char *buffer,unsigned long timeout)
{
	int n = 0;
	int index = 0;	
	unsigned long startTime = millis();

	while((millis() - startTime) < timeout)
	{
		n = Serial1.available();
		if (n > 0)
		{
			for (int i = 0; i < n; i++)
			{
				
				buffer[index] = Serial1.read();
				Serial.print(buffer[index]);
				index++;
			}
		}
	}

	return index;
}

void DS1::resetRecv()
{
	int len = 0, i = 0;
	char tempByte;

	delay(100);
	len = Serial1.available();
	if (len>0)
	{
		for (i = 0; i < len; i++)
		{			
			tempByte = Serial1.read();
		}
	}
}
int DS1::handleDevData()
{
   int len = Serial1.available();
   if(len>0)
   {
    delay(100);
    len = Serial1.available();
    Serial.print("Recv:");
    Serial.println(len);
   }

   return len;
}


void DS1::myWatchdogEnable(const byte interval) 
  {  
  MCUSR = 0;                          // reset various flags
  WDTCSR |= 0b00011000;               // see docs, set WDCE, WDE
  WDTCSR =  0b01000000 | interval;    // set WDIE, and appropriate delay

  wdt_reset();
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
  sleep_mode();            // now goes to Sleep and waits for the interrupt
  } 
