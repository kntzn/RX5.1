// 
// 
// 

#include "MotorController.h"

int MotorController::ecoModeThrottle (int thrIn, int current_speed, double dt)
    {
    int thr = 0;
    int maxThrIncr = (POWER_MAX_ECO - THR_POWER_START) * ((dt/1000.0)/(RAMP_UP_T_ECO));

    if (thrIn < THR_BRAKE_START)
        thr = map (thrIn, THR_MIN, THR_BRAKE_START, BRAKE_MAX_ECO, THR_BRAKE_START);
    else if (thrIn > THR_POWER_START)
        {
        thr = map (thrIn, THR_POWER_START, THR_MAX, THR_POWER_START, POWER_MAX_ECO);
        // Chops throttle if it rises rapidly
        if (thr - throttleOutput > maxThrIncr && throttleOutput > THR_MID)
            thr = throttleOutput + maxThrIncr;
        }
    else
        thr = THR_MID;

    return thr;
    }

MotorController::MotorController (uint8_t PPM_pin, mode ride_mode):
    isCruiseActive (false),
    throttleOutput (THR_MID),
    rideMode (ride_mode),
    cruiseSpeed (0),
    previous_thr_time (millis ())
    {
    motor.attach (PPM_pin);
    motor.writeMicroseconds (THR_MID);
    }

void MotorController::update (int throttle, mode current_mode, 
                              bool cruise, double current_speed)
    {
    double dt = millis () - previous_thr_time;
    previous_thr_time = millis ();

    // Cruise mode
    //        gets activated
    if (!isCruiseActive && cruise)
        {
        isCruiseActive = true;
        cruiseSpeed = current_speed;
        }
    //        gets deactivated
    if (isCruiseActive && !cruise)
        {
        isCruiseActive = false;
        cruiseSpeed = 0;
        }

    // Throttle control
    switch (rideMode)
        {
        case mode::lock:
            throttleOutput = THR_MIN;
            break;
        case mode::hybrid:
            break;
        case mode::eco:
            throttleOutput = ecoModeThrottle (throttle, current_speed, dt);
            break;
        case mode::cruise:
            break;
        case mode::sport:
            break;
        default:
            break;
        }
    }
