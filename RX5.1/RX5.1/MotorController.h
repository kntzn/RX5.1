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
    bool isCruiseActive;
    mode rideMode;
    double cruiseSpeed;
    Servo motor;
    
public:
     MotorController (uint8_t PPM_pin, mode ride_mode);

     void update (int throttle, mode current_mode,
                  bool cruise, double current_speed);
};

#endif

