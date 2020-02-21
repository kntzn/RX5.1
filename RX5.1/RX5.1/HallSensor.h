// HallSensor.h

#ifndef _HALLSENSOR_h
#define _HALLSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SysConfig.h"


// Those variables and methods are public
// to allow attachInterrupt use this handler
namespace _HS
    {
    unsigned long int last_hs_sensed, dt;

    void _hndlr ()
        {
        dt = millis () - last_hs_sensed;
        last_hs_sensed += dt;
        }
    };

class HallSensor
    {
    private:
        double speed, accel;
    public:
        HallSensor (uint8_t hs_interrupt);

        void update ();

        double getSpeed ();
        double getAccel ();
    };

#endif

