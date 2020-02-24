// DriveController.h

#ifndef _MOTORCONTROLLER_h
#define _MOTORCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <Servo.h>
#include "SysConfig.h"
#include "Config.h"

// TODO: fix speed limit & ramp up

class MotorController
    {
    private:
        mode rideMode;
        Servo motor;

        int throttleOutput;
        unsigned long previous_thr_time, last_avail;

        // Input signal converters
        int hybridModeThrottle (int thrIn, int current_speed);
        int    ecoModeThrottle (int thrIn, int current_speed, double dt);
        int cruiseModeThrottle (int thrIn, int current_speed, double dt);

        // Failsafe handler
        void failSafe ();
        // Changes mode if possible
        void updateMode (int throttle, mode input_mode,
                         int current_speed);

    public:
         MotorController (uint8_t PPM_pin, mode ride_mode);

         // Main motor handler function
         void update (int throttle, mode current_mode, double current_speed);

         // Returns output to PPM channel
         int getThrottleOutput ();
    };

#endif

