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

void MotorController::failSafe ()
    {
    // Time scince failsafe activation
    unsigned long delta = (millis () - last_avail) - FAILSAFE_MS;

    // Ramp up process
    if (delta < FAILSAFE_RAMP_UP)
        {
        // Ramp up percents
        float k = float (delta) / float (FAILSAFE_RAMP_UP);
        // Slide from initial signal to full lock
        int rampDownSignal = (1.f - k) * (throttleOutput) +
            k  * (PPM_MIN);

        motor.writeMicroseconds (rampDownSignal);
        }
    // Full motor lock
    else
        motor.writeMicroseconds (PPM_MIN);
    }
void MotorController::updateMode (int throttle, mode input_mode, 
                                  int current_speed)
    {
    if (throttle < THR_MID + THR_DELTA_TO_MAX*VESC_DEADBAND &&
        throttle > THR_MID - THR_DELTA_TO_MIN*VESC_DEADBAND)
            switch (input_mode)
                {
                case mode::lock:
                    if (current_speed < SPEED_MAX_LCK)
                        rideMode = input_mode;
                    break;
                case mode::hybrid:
                    if (current_speed < SPEED_MID_HYB)
                        rideMode = input_mode;
                    break;
                case mode::eco:
                case mode::cruise:
                case mode::sport:
                    rideMode = input_mode;
                    break;
                default:
                    break;
                }
    }

MotorController::MotorController (uint8_t PPM_pin, mode ride_mode):
    throttleOutput (THR_MID),
    rideMode (ride_mode),
    previous_thr_time (millis ()),
    last_avail (millis ())
    {
    motor.attach (PPM_pin);
    motor.writeMicroseconds (THR_MID);
    }

void MotorController::update (int throttle, mode current_mode, double current_speed)
    {
    // Timers
    double dt = millis () - previous_thr_time; // Gets time delta for ramp up
    previous_thr_time = millis ();             // Starts timer once again
    if (throttle)                              // |
        last_avail = millis ();                // |Keeps failsafe deactivated

    // Updates mode according to limitations
    updateMode (throttle, current_mode, current_speed);
    
    // Throttle control
    if (millis () - last_avail < FAILSAFE_MS)
        {
        if (throttle)
            {
            // Converts input signal to output throttle according to the mode
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
            }
        motor.writeMicroseconds (throttleOutput);
        }
    // And failsafe
    else
        failSafe ();


        
    }
int MotorController::getThrottleOutput ()
    {
    return motor.readMicroseconds ();
    }
