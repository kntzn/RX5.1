// 
// 
// 

#include "MotorController.h"

MotorController::MotorController (uint8_t PPM_pin, mode ride_mode):
    isCruiseActive (false),
    rideMode (ride_mode),
    cruiseSpeed (0)
    {
    motor.attach (PPM_pin);
    }

void MotorController::update (int throttle, mode current_mode, 
                              bool cruise, double current_speed)
    {


    }
