// Communication.h

#ifndef _COMMUNICATION_h
#define _COMMUNICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "cobs.h"

class Communication
{
 protected:


	 uint8_t channelId, setPin;
 public:
	 Communication(uint8_t set_pin, uint8_t channel);

};

#endif

