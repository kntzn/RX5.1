// 
// 
// 

#include "Communication.h"

Communication::Communication(uint8_t set_pin, uint8_t channel) :
	setPin(set_pin),
	channelId(channel)
	{
	pinMode(set_pin, OUTPUT);
	digitalWrite(set_pin, HIGH);
	}
