#include "Comm.h"
#include "cobs.h"
#include "Pinout.h"

void Communication::sendPacket (uint8_t* pack, size_t len)
    {
    assert (len <= PACK_SIZE_MAX);

    uint8_t cobsEncodedPack [PACK_SIZE_MAX + 1] = { };
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

void Communication::activateRawinput ()
    {
    rawinput = true;
    }

void Communication::flush ()
    {
    inputBuf.clear ();
    }

Communication::Communication ():
    rawinput (false)
    {
    }

void Communication::sendCommand (command cmd, uint16_t arg)
    {
    buffer [0] = static_cast <char> (cmd);
    buffer [1] = arg / 256;
    buffer [2] = arg % 256;

    sendPacket (buffer, PACK_SIZE_DEFAULT);
    }

void Communication::sendRequest (command req)
    {
    buffer [0] = static_cast <char> (req);
    
    sendPacket (buffer, PACK_SIZE_DEFAULT);
    }

void Communication::sendResponse (response resp, uint16_t val)
    {
    buffer [0] = static_cast <char> (resp);
    buffer [1] = val / 256;
    buffer [2] = val % 256;

    for (int i = 0; i < RESPONSE_PACKETS; i++)
        sendPacket (buffer, PACK_SIZE_DEFAULT);
    }

Communication::command Communication::receiveRequest ()
    {
    // If new packet's size is correct
    if (receivePacket (buffer) == PACK_SIZE_DEFAULT)
        { 
        return static_cast <command> (buffer [0]);
        }
    return command::nocmd;
    }

Communication::response Communication::receiveResponse ()
    {
    // If new packet's size is correct
    if (receivePacket (buffer) == PACK_SIZE_DEFAULT)
        {
        return static_cast <response> (buffer [0]);
        }
    return response::noresp;
    }

bool Communication::rawinputActive ()
    {
    return rawinput;
    }

uint8_t * Communication::argbuf ()
    {
    return (buffer + 1);
    }