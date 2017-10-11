#include <avr/sleep.h>
#include <avr/wdt.h>
#include "S76S.h"

//public
S76S::S76S()
{	
	
}


//---------------S76S-----------//
//public
void S76S::Init(RFType type)
{
   Serial.begin(BaudRate);
   Serial1.begin(BaudRate);
  int cmdIndex = 0;
  char *cmd[] = { "LoraAutoBoot 0\r","LoraStartWork DISABLE\r","SetSystemMode inNormal\r","LoraMode SLAVE\r","LoraMode MASTER\r" };

  delay(1000);
  resetRecv();

  Serial.print("LoraAutoBoot close");
  cmdIndex = 0;
  while (!SendCommand(cmd[cmdIndex])) { delay(1000); }
  delay(400);
  
  Serial.print("LoraStartWork DISABLE");
  cmdIndex = 1;
  while (!SendCommand(cmd[cmdIndex])) { delay(1000); }
  delay(400);
  
  Serial.print("LoraSystemMode inNormal");
  cmdIndex = 2;
  while (!SendCommand(cmd[cmdIndex])) { delay(1000); }
  delay(400);
  
  switch (type)
  {
  case RFType_S76S_Slave:

    Serial.print("LoraSystemMode Slave");
    cmdIndex = 3;

    while (!SendCommand(cmd[cmdIndex])) { delay(1000); }
    delay(400);
    break;
    
  case RFType_S76S_Master:

    Serial.print("LoraSystemMode Master");
    cmdIndex = 4;

    while (!SendCommand(cmd[cmdIndex])) { delay(1000); }
    delay(400);
    break;
  
  }

  Serial.println("RF module init finish");
}


//public
bool S76S::SetAddress(char *address)
{
    char blank = ' ';
    char *cmd = "LoraSetMyAddr";
    delay(1000);
    resetRecv();
    
    Serial1.write(cmd, strlen(cmd));
    Serial1.write(blank);
    Serial1.write(address, strlen(address));
    Serial1.write(cmdEndSymbol);
    ack = GetSerialData();

    //debug message
    Serial.println(ack);   
    Serial.print(cmd);
    Serial.print(blank);
    Serial.print(address);
    Serial.println(cmdEndSymbol);
     
    if(ack.equals("OK"))
    {
      Serial.println("My Adreess set successful!");
      return true;
    }
    else
    {
      Serial.println("My Adreess set fail!");
      return false;
    }
   
}
//public
bool S76S::SetGateWayAddress(char *address)
{
    char blank = ' ';
    char *cmd = "LoraSetGateWayAddr";
    
    delay(1000);
    resetRecv();
    
    Serial1.write(cmd, strlen(cmd));
    Serial1.write(blank);
    Serial1.write(address, strlen(address));
    Serial1.write(cmdEndSymbol);
    ack = GetSerialData();

    //debug message
    Serial.println(ack); 
    Serial.print(cmd);
    Serial.print(blank);
    Serial.print(address);
    Serial.println(cmdEndSymbol);
     
    
    if(ack.equals("OK"))
    {
      Serial.println("Gateway Adreess set successful!");
      return true;
    }
    else
    {
      Serial.println("Gateway Adreess set fail!");
      return false;
    }
   
}
//public
bool S76S::StartWork(bool work)
{
    char *cmd[] = {"LoraStartWork DISABLE\r","LoraStartWork ENABLE\r"};
    int cmdIndex = 0;
    
    delay(1000);
    resetRecv();

    if(work)
      cmdIndex = 1;
    else
      cmdIndex = 0;
    
    Serial1.write(cmd[cmdIndex], strlen(cmd[cmdIndex]));
    ack = GetSerialData();

    //debug message
    Serial.println(ack); 
    Serial.print(cmd[cmdIndex]);
    Serial.println(cmdEndSymbol);
        
    if(ack.equals("OK") && (work == true))
    {
      Serial.println("Lora Start Work!");
      return true;
    }
    else if(ack.equals("OK") && (work == false))
    {
      Serial.println("Lora Stop Work");
      return true;
    }
    else
    {
      Serial.println("Command Error");
      return false;
    }
   
}

//public
bool S76S::AddSlaveNode(char *address,char *index)
{
  bool isSucceed = 0;
  char *cmd = "LoraAddSlaveNode ";
  int recvLen = 0;
  const int maxBufSize = 20;
  char recvBuf[maxBufSize] = { 0 };

  delay(1000);
  resetRecv();

  Serial.print("Add node:[");
  Serial.print(address);
  Serial.println("]");
  

  Serial1.write(cmd, strlen(cmd));
  Serial1.write(address, strlen(address));
  Serial1.write(cmdEndSymbol);

  Serial.print(cmd);
  Serial.print(address);
  Serial.println(cmdEndSymbol);

  recvLen = readDataFromSerial(recvBuf, 100);
  if (recvLen == 2 && strcmp("OK", recvBuf) == 0)
  {
      Serial.println("Add OK!"); 
      if(SaveSlaveNode(index))
        return true;
      else
        return false;           
  }
  else
  {
    Serial.println("Add Fail!");
    return false;
  }
  

}
bool S76S::SaveSlaveNode(char *index)
{
  int recvLen = 0;
  const int maxBufSize = 20;
  char recvBuf[maxBufSize] = { 0 };
  char *cmd = "LoraSaveSlaveNode ";

  delay(1000);
  resetRecv();
  Serial1.write(cmd, strlen(cmd));
  Serial1.write(index, strlen(index));
  Serial1.write(cmdEndSymbol);

    //debug message
    Serial.print(cmd);
    Serial.print(index);
    Serial.println(cmdEndSymbol);
  
  recvLen = readDataFromSerial(recvBuf, 100);
  
  if (recvLen == 2 && strcmp("OK", recvBuf) == 0)
  {
      Serial.print("Save ");
      Serial.print(index);
      Serial.println(" OK");
      return true;    
  }
  else
  {
    Serial.println("Save Fail!");
    return false;
  }
}
bool S76S::RemoveSlaveNode(char *index)
{
  int recvLen = 0;
  const int maxBufSize = 20;
  char recvBuf[maxBufSize] = { 0 };
  //char *number = byte(index);
  char *cmd = "LoraRemoveSlaveNode ";

  delay(1000);
  resetRecv();
  Serial1.write(cmd, strlen(cmd));
  Serial1.write(index, strlen(index));
  Serial1.write(cmdEndSymbol);

  //debug message
  Serial.print(cmd);
  Serial.print(index);
  Serial.println(cmdEndSymbol);
  recvLen = readDataFromSerial(recvBuf, 100);
  
  if (recvLen == 2 && strcmp("OK", recvBuf) == 0)
  {
      Serial.print("Remove ");
      Serial.print(index);
      Serial.println(" OK");
      return true;    
  }
  else
  {
    Serial.println("Command Fail!");
    return false;
  }
}


//public
void S76S::MasterDownlink(char *address,char *data)
{
    char *cmd = "LoraMasterPld";
    Serial1.write(cmd, strlen(cmd));
    Serial1.write(' ');
    Serial1.write(address, strlen(address));
    Serial1.write(' ');
    Serial1.write(data, strlen(data));
    Serial1.write(cmdEndSymbol);

    ack = GetSerialData();
    Serial.println(ack);
}

//public
void S76S::SlaveUplink(char *buffer,int dataLength)
{
    char *mode = "2";
    
    char *cmd = "LoraSlavePld";
    
    Serial1.write(cmd, strlen(cmd));
    Serial1.write(' ');
    Serial1.write('2');
    Serial1.write(' ');
    Serial1.write(buffer, dataLength);
    Serial1.write(cmdEndSymbol);
/*
    Serial.print(cmd);
    Serial.print(' ');
    Serial.print('2');
    Serial.print(' ');
    Serial.print(buffer);
    Serial.println(cmdEndSymbol);
*/
    ack = GetSerialData();
    Serial.println(ack);
    resetRecv();
}
bool S76S::SendCommand(char *cmd)
{
  bool succeed = 0;
  int recvLen = 0;
  const int maxBufSize = 20;
  char recvBuf[maxBufSize] = { 0 };

  Serial1.write(cmd, strlen(cmd));
  memset(recvBuf, 0, maxBufSize);
  recvLen = readDataFromSerial(recvBuf, 100);
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

String S76S::GetSerialData(void)
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
    }
  }
  return data;
}

int S76S::readDataFromSerial(char *buffer,unsigned long timeout)
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

void S76S::resetRecv()
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

//MQTT
void S76S::Publish(String sendPayload, byte topicId)
{
  sendBuffer[0] = (byte)sendPayload.length() + MQTT_HEADER_SIZE;
  sendBuffer[1] = (byte)PublishType;
  sendBuffer[2] = topicId; //topic id
  
  for (int i = 0; i<sendPayload.length(); i++)
  {
      sendBuffer[i + MQTT_HEADER_SIZE] = sendPayload.charAt(i);
  }
 
   SlaveUplink(sendBuffer, sendPayload.length() + MQTT_HEADER_SIZE);
}

void S76S::MultiPublish(int publishCount, String sendPayload[], byte *topicId)
{
  int index = 0;
  for (int i = 0; i<publishCount; i++)
  {
    sendBuffer[index++] = (byte)sendPayload[i].length() + MQTT_HEADER_SIZE;
    Serial.print(sendBuffer[index - 1]);
    Serial.print(" ");

    sendBuffer[index++] = (byte)PublishType;
    Serial.print(sendBuffer[index - 1]);
    Serial.print(" ");

    sendBuffer[index++] = topicId[i]; //topic id
    Serial.print(sendBuffer[index - 1]);
    Serial.print(" ");


    for (int j = 0; j<sendPayload[i].length() ; j++)
    {
      sendBuffer[index++] = sendPayload[i].charAt(j);
      Serial.print(sendBuffer[index - 1]);
      Serial.print(" ");
    }
  }
  Serial.println();
  SlaveUplink(sendBuffer, index);
}

void S76S::sendDataTimes(int PublishCount ,String sendBuffer[],byte *topicID ,int times) 
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
int S76S::handleDevData()
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
