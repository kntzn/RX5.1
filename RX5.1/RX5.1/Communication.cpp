// 
// 
// 

#include "Communication.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "cobs.h"

#define StartBlock()	(code_ptr = dst++, code = 1)
#define FinishBlock()	(*code_ptr = code)


void Communication::sendPacket (uint8_t* pack, size_t len)
    {
    assert (len <= PACK_SIZE_MAX);

    uint8_t cobsEncodedPack [PACK_SIZE_MAX + 1] = {};
    size_t cobsEncodedPackLen =
        COBS::encode (pack, len, cobsEncodedPack);

    // Actual packet
    Serial.write (cobsEncodedPack, cobsEncodedPackLen);
    // EOP byte
    Serial.write (EOP);
    }

size_t Communication::receivePacket (uint8_t * pack)
    {
    // Reads the serial
    while (Serial.available ())
        inputBuf.push_back (Serial.read ());

    // Searches for the EOP symb.
    int eop = -1;
    for (int i = 0; i < inputBuf.size (); i++)
        if (inputBuf [i] == EOP)
            {
            eop = i;
            break;
            }

    // Decodes the message if it is avail.
    // Fix: prevents corrupted packets being decoded 
    //              out of range of the "pack" array
    size_t cobsDecodedPackLen = 0;
    if (eop != PACK_SIZE_MAX + 1)
        {
        cobsDecodedPackLen =
            COBS::decode (inputBuf.data (), eop, pack);
        }

    // Removes the message from the buffer
    if (eop != -1)
        {
        size_t initial_size = inputBuf.size ();

        uint8_t* data = inputBuf.data ();

        // Buffer shift
        for (int i = 0; i < initial_size - eop - 1; i++)
            data [i] = data [i + eop + 1];

        for (int i = 0; i < eop + 1; i++)
            inputBuf.pop_back ();

        }

    return cobsDecodedPackLen;
    }


Communication::Communication (uint8_t set_pin, uint8_t channel):
    setPin (set_pin),
    channelId (channel)
    {
    pinMode (set_pin, OUTPUT);
    changeCh (channel);
    digitalWrite (set_pin, HIGH);
    }

void Communication::changeCh (uint8_t ch)
    {
    // turns AT mode on
    digitalWrite (setPin, LOW);
    delay (AT_MODE_ON_DELAY);

    String cmdStr = "AT+C";
    uint8_t channel = ch%N_CHANNELS;

    // Adds "xxx" part to the "AT+C" string
    if (channel < 10)
        {
        cmdStr += "00";
        cmdStr += String (channel);
        Serial.println (cmdStr.c_str ());
        }
    else if (channel < 100)
        {
        cmdStr += "0";
        cmdStr += String (channel);
        Serial.println (cmdStr.c_str ());
        }

    // turns AT mode off
    digitalWrite (setPin, HIGH);
    delay (AT_MODE_OFF_DELAY);
    }
