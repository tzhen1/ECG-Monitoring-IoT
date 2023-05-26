#include <Arduino.h>

typedef enum 
{
	TEMPERATURE = 0x00,
	CONFIGURATION = 0x01,
	T_HYST = 0x02,
	T_OS = 0x03,
} MAX30205_Registers;

class MAX30205 
{
public:
	MAX30205();

	void begin(uint8_t address);

	float readTemperature();
	float readT(); // short-cut for readTemperature

private:
	uint8_t _address;
	uint16_t readData(uint8_t pointer);
};
