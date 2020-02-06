// HallSensor.h

#ifndef _HALLSENSOR_h
#define _HALLSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SysConfig.h"

double speed = 0.0;
unsigned long int last_hs_sesnsed = millis ();

void hsInterruptHandler ()
    {
    speed = (WHEEL_DIA * M_PI) /
        ((millis () - last_hs_sesnsed) / 1000.0) *
        3.6;

    last_hs_sesnsed = millis ();
    }

double getSpeed ()
    {
    if (millis () - last_hs_sesnsed > 500)
        return 0.0;

    return speed;
    }
#endif

