// Communication.h

#ifndef _COMMUNICATION_h
#define _COMMUNICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Sarray.h"
#include "Pinout.h"

#define PACK_SIZE_MAX 64
#define PACK_SIZE_DEFAULT 10
#define EOP 0x00
#define N_CHANNELS 100
#define AT_MODE_ON_DELAY 80
#define AT_MODE_OFF_DELAY 40


#define REQUEST_PERIOD 500
#define RESPONSE_TIMEOUT 100
#define REQUEST_TIMEOUT 500
#define RESPONSE_PACKETS 5

class Communication
    {
    public:
        enum class command
            {
            nocmd    =  0,  // no                            cmd avail
            rawinput = 'R', // reserved                      cmd
            changeCh = 'C', // change channel                cmd
            motor    = 'M', // change throttle, mode, cruise cmd
            lights   = 'L', // change Llghts state           cmd
            contTrip = 'N', // Start new / continue trip     cmd
            trip     = 'T', // trip controller state         request
            battery  = 'B'  // battery         state         request
            };

        enum class response
            {
            noresp  = 0,   // no              response avail
            trip    = 't', // trip controller response
            battery = 'b', // bms             response
            };

    private:    
        sarray <uint8_t, PACK_SIZE_MAX + 1> inputBuf;
        uint8_t buffer [PACK_SIZE_MAX];

        uint8_t channelId, setPin;

        void sendPacket (uint8_t* pack, size_t len); // private function to write packet
        size_t receivePacket (uint8_t* pack);        // private function to read  packet

    public:
        Communication (uint8_t set_pin, uint8_t channel);

        void changeCh (uint8_t ch);

        // TODO:

        void sendCommand(command cmd);
        void sendResponse(response rsp);

        command receiveRequest();
        response receiveResponse();

        //void activateRawinput();
        //void flush();

        //bool rawinputActive();
        // Returns ptr to the args. array
        uint8_t *argbuf();

    };

#endif

