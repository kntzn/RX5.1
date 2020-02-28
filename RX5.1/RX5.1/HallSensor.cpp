// 
// 
// 

#include "HallSensor.h"

HallSensor::HallSensor (uint8_t hs_interrupt):
    new_turns (0),
    speed (0.0),
    accel (0.0)
    {
    _HS::last_hs_sensed = micros ();
    _HS::dt = 0;
    _HS::new_turns = 0;

    attachInterrupt (hs_interrupt, _HS::_hndlr, FALLING);
    }

void HallSensor::update ()
    {
    new_turns += _HS::new_turns;
    
    double new_speed;
    if (new_turns)
        new_speed = (((double) (_HS::new_turns * WHEEL_DIA * M_PI))/
                     ((double) (_HS::dt) / 1000000.0)) * 3.6;
    else
        new_speed = speed;

    //accel = (new_speed - speed) / (double)_HS::dt;
    accel = 0.0;
    speed = new_speed;
    _HS::new_turns = 0;
    }

double HallSensor::getSpeed ()
    {
    if (micros () - _HS::last_hs_sensed > 1000000) // 1 second
        return 0.0;

    return speed;
    }
double HallSensor::getAccel ()
    {
    if (_HS::dt > 500)
        return 0.0;

    return accel;
    }

int HallSensor::getNewTurnsAmount ()
    {
    int tmp = new_turns;
    new_turns = 0;
    return tmp;
    }
