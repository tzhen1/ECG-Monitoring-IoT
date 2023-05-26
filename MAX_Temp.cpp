#include <Wire.h>
#include "MAX_Temp.h"
#define TEMP_SDA 25
#define TEMP_SCL 22
#define BUZZER 12 // buzzer

float validTemp = 0; // normal temp
int capture_temp_abnormal = 0;

MAX30205::MAX30205(){}

void MAX30205::begin(uint8_t address) 
{
	_address = address;
	Wire.begin(TEMP_SDA, TEMP_SCL);
	Wire.beginTransmission(_address);
	Wire.write(CONFIGURATION); //0x01
	Wire.write(0x0); // temp
	Wire.endTransmission();

  
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  
}

float MAX30205::readT() {
	return readTemperature();
}

float MAX30205::readTemperature() 
{
  uint16_t rawT = readData(TEMPERATURE);

  if( (rawT * 0.00390625) < 200 && (rawT * 0.00390625) > 10) // temp >10, equation to find real temp
  {
    validTemp = (rawT * 0.00390625);
  }
  else if(capture_temp_abnormal == 0)
  {
    // buzz 
    digitalWrite(BUZZER, HIGH);
    delay(10);
    digitalWrite(BUZZER, LOW);
    capture_temp_abnormal = 1;

  }
  
  return validTemp;
  delay(300);
}

uint16_t MAX30205::readData(uint8_t pointer) 
{
	Wire.beginTransmission(_address);
	Wire.write(pointer);
	Wire.endTransmission();

	delay(10);
	Wire.requestFrom(_address, (uint8_t)2);

	byte msb = Wire.read();
	byte lsb = Wire.read();

	return msb << 8 | lsb;
}