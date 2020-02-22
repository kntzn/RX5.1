// BMS.h

#ifndef _BMS_h
#define _BMS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "VoltageCurve.h"

class BMS
    {
    private:
        uint8_t readPin;
        double bat_voltage, percents, whDrawn;

        
        long readVcc ();
        // Returns average analog reading
        double aver_analog (uint32_t times = 10U);
        
        void readVoltage (bool noFilter = false);
        void getPercentsFromVoltage ();
        void getUsageFromPercents ();
 
    public:
        BMS (uint8_t ADCpin);

        void update (int throttle);

        // getters 
        double getCellVoltage ();
        double getBatVoltage ();
        double getPercents ();
        double getWhDrawn ();
        double getWhLeft ();

    };

#endif

