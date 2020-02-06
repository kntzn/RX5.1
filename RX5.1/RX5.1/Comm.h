// Comm.h


#ifndef _COMM_h
#define _COMM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <stdint.h>
#include <stddef.h>

#define StartBlock()	(code_ptr = dst++, code = 1)
#define FinishBlock()	(*code_ptr = code)

#include "sarray.h"
#include <assert.h>

#define PACK_SIZE_MAX 64
#define PACK_SIZE_DEFAULT 3
#define EOP 0x00

#define REQUEST_PERIOD 500
#define RESPONSE_TIMEOUT 100
#define REQUEST_TIMEOUT 500
#define RESPONSE_PACKETS 5

class Communication
    {
    private:
        sarray <uint8_t, PACK_SIZE_MAX + 1> inputBuf;
        uint8_t buffer [PACK_SIZE_MAX];
        
        bool rawinput;

    public:
        enum class command
            {
            nocmd        =  0 ,
            throttle     = 'T',
            voltage      = 'V',
            mode         = 'M',
            speed        = 'S',


            raw          = 'R',
            raw_safety   = 'RR'
            };
        
        enum class response
            {
            noresp       =  0,
            voltage      = 'v',
            speed        = 's'
            };

        Communication ();        

        void sendCommand (command cmd, uint16_t arg = 0);
        void sendRequest (command req);

        void sendResponse (response resp, uint16_t val = 0);

        command receiveRequest ();
        response receiveResponse ();

        void sendPacket (uint8_t* pack, size_t len);
        size_t receivePacket (uint8_t* pack);

        void activateRawinput ();
        void flush ();


        bool rawinputActive ();
        // Returns ptr to the args. array
        uint8_t *argbuf ();

    };

#endif

