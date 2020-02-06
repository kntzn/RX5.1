
// Battery.h

#pragma once
#include "Config.h"
#include "Pinout.h"


#ifndef _BATTERY_h
#define _BATTERY_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class Battery
    {
    private:
        double bat_voltage;

        long aver_analog (uint8_t pin, byte times);

        long readVcc ();
           
        byte in_pin;

    public:
        Battery (byte pin);

        void batMeasure ();
            
        double getBatVoltage ();
            

    };


#endif







