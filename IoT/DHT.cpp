/* DHT library 

MIT license
written by Adafruit Industries
*/

#include "DHT.h"
#define NAN 0

DHT::DHT(uint8_t pin, uint8_t type, uint8_t count) 
{
  _pin = pin;
  _type = type;
  _count = count;  
  firstreading = true;
}

void DHT::begin(uint8_t type)
{
  // set up the pins!
  pinMode(_pin, INPUT);
  digitalWrite(_pin, HIGH);
  _type = type;
  _lastreadtime = 0;
}

//boolean S == Scale.  True == Farenheit; False == Celcius
float DHT::readTemperature(bool S) 
{
  float f;

  if (read()) {
    switch (_type) 
	{
    case DHT11:
      f = data[2];
      if(S)
      	f = convertCtoF(f);
      return f;
    case DHT22:
    case DHT21:
      f = data[2] & 0x7F;
      f *= 256;
      f += data[3];
      f /= 10;
      if (data[2] & 0x80)
	f *= -1;
      if(S)
	f = convertCtoF(f);

      return f;
    }
  }
  Serial.print("Temperature read fail");
  return 0;
  //return NAN;
}

float DHT::convertCtoF(float c) {
	return c * 9 / 5 + 32;
}

float DHT::readHumidity(void) {
  float f;
  if (read()) {
    switch (_type) {
    case DHT11:
      f = data[0];
      return f;
    case DHT22:
    case DHT21:
      f = data[0];
      f *= 256;
      f += data[1];
      f /= 10;
      return f;
    }
  }
  Serial.print("Read fail");
  return NAN;
}


boolean DHT::read(void) {
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  uint8_t max,min;
  unsigned long currenttime;

  // pull the pin high and wait 250 milliseconds
  digitalWrite(_pin, HIGH);
  delay(250);

  currenttime = millis();
  if (currenttime < _lastreadtime) {
    // ie there was a rollover
    _lastreadtime = 0;
  }
  if (!firstreading && ((currenttime - _lastreadtime) < 2000)) {
    return true; // return last correct measurement
    //delay(2000 - (currenttime - _lastreadtime));
  }
  firstreading = false;
  /*
    Serial.print("Currtime: "); Serial.print(currenttime);
    Serial.print(" Lasttime: "); Serial.print(_lastreadtime);
  */
  _lastreadtime = millis();

  
  
  // now pull it low for ~20 milliseconds
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(20);
  //cli();
  digitalWrite(_pin, HIGH);
  delayMicroseconds(40);
  pinMode(_pin, INPUT);
  

  uint8_t myCounter[MAXTIMINGS]={0};
  // read in timings
  i=0;
  max=0;
  min=255;
  while(1)
  {
    counter = 0;
	if(digitalRead(_pin)==LOW)
	{
		//delayMicroseconds(1);
		continue;
	}
	else
	{
		while (digitalRead(_pin) == HIGH) 
		{
		  counter++;
		  //delayMicroseconds(1);
		  if (counter >= 20) 
		  {
			break;
		  }
		}
	}
		    
	myCounter[i] = counter;
	
	i++;
    //laststate = digitalRead(_pin);
    if (counter >= 20) 
	{		
		break;
	}	

	if(counter>max)
		max=counter;
	if(counter<min)
		min=counter;
  }

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  //int k=0;
  uint8_t index=0;
  uint8_t bitCount=0;
  uint8_t threshold=min+((max-min)/2);
  //Serial.print("T=");
  //Serial.println(threshold);
  for(i=1;i<41;i++)
  {	  
	  index=(i-1)/8;	  	 
	  //Serial.print(myCounter[i]);
	  //Serial.print(":");
	  
	  if(myCounter[i]>threshold)
	  {
		  data[index]|=1;		  
	  }
	  else
	  {
		  data[index]&=0xFE;
	  }

	  bitCount++;

	  if(bitCount<8)
	  {
		  data[index]=data[index]<<1;
	  }
	  else
	  {
		  bitCount=0;
	  }	  	  
  }
  //Serial.println();
  
  //sei();
  
  /*
  Serial.println(j, DEC);
  Serial.print(data[0], HEX); Serial.print(", ");
  Serial.print(data[1], HEX); Serial.print(", ");
  Serial.print(data[2], HEX); Serial.print(", ");
  Serial.print(data[3], HEX); Serial.print(", ");
  Serial.print(data[4], HEX); Serial.print(" =? ");
  Serial.println(data[0] + data[1] + data[2] + data[3], HEX);
  */

  // check we read 40 bits and that the checksum matches
  
  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
  {
    return true;
  }
  

  return false;

}
