// DriveController.h

#ifndef _MOTORCONTROLLER_h
#define _MOTORCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SysConfig.h"
#include <Servo.h>

class MotorController
    {
    private:
        mode rideMode;
        Servo motor;

        bool isCruiseActive;
        double cruiseSpeed;

        int throttleOutput;
        unsigned long previous_thr_time;

        int hybridModeThrottle (int thrIn, int current_speed);
        int    ecoModeThrottle (int thrIn, int current_speed, double dt);
        int cruiseModeThrottle (int thrIn, int current_speed, double dt);

    public:
         MotorController (uint8_t PPM_pin, mode ride_mode);

         void update (int throttle, mode current_mode,
                      bool cruise, double current_speed);
    };

#endif

