// 
// 
// 

#include "HallSensor.h"

HallSensor::HallSensor (uint8_t hs_interrupt):
    new_turns (0),
    speed (0.0),
    accel (0.0)
    {
    _HS::last_hs_sensed = millis ();
    _HS::dt = 0;
    _HS::new_turns = 0;

    attachInterrupt (hs_interrupt, _HS::_hndlr, FALLING);
    }

void HallSensor::update ()
    {
    new_turns += _HS::new_turns;
    _HS::new_turns = 0;

    double new_speed = (((double) (new_turns * WHEEL_DIA * M_PI))/
                        ((double) (_HS::dt) / 1000.0)) * 3.6;

    accel = (new_speed - speed) / (double)_HS::dt;
    speed = new_speed;
    }

double HallSensor::getSpeed ()
    {
    if (_HS::dt > 500)
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
