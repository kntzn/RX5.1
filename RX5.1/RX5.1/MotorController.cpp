// 
// 
// 

#include "MotorController.h"

int MotorController::hybridModeThrottle (int thrIn, int current_speed)
    {
    if (current_speed < SPEED_MIN_HYB)
        return THR_MID;
    else if (current_speed < SPEED_MID_HYB)
        return map (current_speed,
                    SPEED_MIN_HYB, SPEED_MID_HYB,
                    POWER_MAX_HYB, THR_POWER_START);
    else if (current_speed < SPEED_MAX_HYB)
        return map (current_speed,
                    SPEED_MID_HYB, SPEED_MAX_HYB,
                    THR_BRAKE_START, BRAKE_MAX_HYB);
    else
        return BRAKE_MAX_HYB;
    }

int MotorController::ecoModeThrottle (int thrIn, int current_speed, double dt)
    {
    int thr = 0;
    //int maxThrIncr = (POWER_MAX_ECO - THR_POWER_START) * ((dt/1000.0)/(RAMP_UP_T_ECO));

    if (thrIn < THR_BRAKE_START)
        thr = map (thrIn, THR_MIN, THR_BRAKE_START, BRAKE_MAX_ECO, THR_BRAKE_START);
    else if (thrIn > THR_POWER_START)
        {
        thr = map (thrIn, THR_POWER_START, THR_MAX, THR_POWER_START, POWER_MAX_ECO);

        // Chops throttle if it rises rapidly
        //if (thr - throttleOutput > maxThrIncr)
        //    thr = throttleOutput + maxThrIncr;
        }
    else
        thr = THR_MID;

    // Speed limiter 
    /*if (current_speed > MAX_SPEED_ECO - CRUISE_SPEED_CUTOFF_START && thr > THR_MID)
        thr = map (current_speed, 
                   MAX_SPEED_ECO - CRUISE_SPEED_CUTOFF_START, MAX_SPEED_ECO,
                   thr,                                       THR_MID);
*/
    return thr;
    }

int MotorController::cruiseModeThrottle (int thrIn, int current_speed, double dt)
    {
    int thr = 0;
    //int maxThrIncr = (POWER_MAX_CRU - THR_POWER_START) * ((dt/1000.0)/(RAMP_UP_T_CRU));

    if (thrIn < THR_BRAKE_START)
        thr = map (thrIn, THR_MIN, THR_BRAKE_START, BRAKE_MAX_CRU, THR_BRAKE_START);
    else if (thrIn > THR_POWER_START)
        {
        thr = map (thrIn, THR_POWER_START, THR_MAX, THR_POWER_START, POWER_MAX_CRU);

        // Chops throttle if it rises rapidly
        //if (thr - throttleOutput > maxThrIncr)
        //    thr = throttleOutput + maxThrIncr;
        }
    else
        thr = THR_MID;

    // Speed limiter 
    /*if (current_speed > MAX_SPEED_CRU - CRUISE_SPEED_CUTOFF_START && thr > THR_MID)
    thr = map (current_speed,
    MAX_SPEED_CRU - CRUISE_SPEED_CUTOFF_START, MAX_SPEED_CRU,
    thr,                                       THR_MID);
    */
    return thr;
    }

MotorController::MotorController (uint8_t PPM_pin, mode ride_mode):
    isCruiseActive (false),
    throttleOutput (THR_MID),
    rideMode (ride_mode),
    cruiseSpeed (0),
    previous_thr_time (millis ()),
    last_avail (millis ())
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

    // Mode switching
    if (throttle < THR_MID + THR_DELTA_TO_MAX*VESC_DEADBAND &&
        throttle > THR_MID - THR_DELTA_TO_MIN*VESC_DEADBAND)
        rideMode = current_mode;

    if (throttle != 0)
        last_avail = millis ();

    // Throttle control + failsafe
    if (millis () - last_avail < FAILSAFE_MS)
        {
        switch (rideMode)
            {
            case mode::lock:
                throttleOutput = THR_MIN;
                break;
            case mode::hybrid:
                throttleOutput = hybridModeThrottle (throttle, current_speed);
                break;
            case mode::eco:
                throttleOutput = ecoModeThrottle (throttle, current_speed, dt);
                break;
            case mode::cruise:
                throttleOutput = cruiseModeThrottle (throttle, current_speed, dt);
                break;
            case mode::sport:
                throttleOutput = throttle;
                break;
            default:
                break;
            }
        motor.writeMicroseconds (throttleOutput);
        }
    else
        {
        // Time scince failsafe activation
        unsigned long delta = (millis () - last_avail) - FAILSAFE_MS;

        // Ramp up process
        if (delta < FAILSAFE_RAMP_UP)
            {
            float k = float (delta) / float (FAILSAFE_RAMP_UP);
            int rampDownSignal = k * PPM_MIN + (1.f - k) * (throttle);

            motor.writeMicroseconds (rampDownSignal);
            }
        // Full motor lock
        else
            motor.writeMicroseconds (PPM_MIN);
        }
    }

int MotorController::getThrottleOutput ()
    {
    return motor.readMicroseconds ();
    }
