// Communication.h

#ifndef _COMMUNICATION_h
#define _COMMUNICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "cobs.h"
#include "Sarray.h"

#define PACK_SIZE_MAX 64
#define PACK_SIZE_DEFAULT 4
#define EOP 0x00

#define REQUEST_PERIOD 500
#define RESPONSE_TIMEOUT 100
#define REQUEST_TIMEOUT 500
#define RESPONSE_PACKETS 5

class Communication
{
 protected:

	 sarray <uint8_t, PACK_SIZE_MAX + 1> inputBuf;
	 uint8_t buffer [PACK_SIZE_MAX];

	 uint8_t channelId, setPin;
 public:
	 Communication(uint8_t set_pin, uint8_t channel);

	 void changeCh(uint8_t ch);

	 // TODO:

	 //void sendCommand(command cmd, uint16_t arg = 0);
	 //void sendRequest(command req);
	 //void sendResponse(response resp, uint16_t val = 0);

	 //command receiveRequest();
	 //response receiveResponse();

	 void sendPacket(uint8_t* pack, size_t len);
	 size_t receivePacket(uint8_t* pack);

	 void activateRawinput();
	 void flush();

	 bool rawinputActive();
	 // Returns ptr to the args. array
	 uint8_t *argbuf();

};

#endif

