// HallSensor.h

#ifndef _HALLSENSOR_h
#define _HALLSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SysConfig.h"

#ifndef _HS
#define _HS
// Those variables and methods are public
// to allow attachInterrupt use this handler
namespace _HS
    {
    unsigned long long int last_hs_sensed, dt;
    int new_turns;

    void _hndlr ()
        {
        if (micros () < last_hs_sensed)
            dt = (unsigned long)(micros () - last_hs_sensed);
        else
            dt = micros () - last_hs_sensed;

        last_hs_sensed = micros ();
        new_turns++;
        }
    };
#endif

class HallSensor
    {
    private:
        int new_turns;
        double speed, accel;
    public:
        HallSensor (uint8_t hs_interrupt);

        void update ();

        double getSpeed ();
        double getAccel ();
        int getNewTurnsAmount ();
    };

#endif

