// 
// 
// 

#include "Lights.h"
#include "Config.h"
#include "SysConfig.h"
#include "kstd.h"

Lights::Lights (uint8_t front_lights_pin, 
                uint8_t rear_lights_pin,
                uint8_t under_lights_pin,
                mode lights_mode):
    fl_pin (front_lights_pin),
    rl_pin (rear_lights_pin),
    ul_pin (under_lights_pin),
    lightsMode (lights_mode)
    {
    pinMode (front_lights_pin, OUTPUT);
    pinMode ( rear_lights_pin, OUTPUT);
    pinMode (under_lights_pin, OUTPUT);

    // TODO: FADE_IN at startup
    /*TIMER_SET (___timer0);

    while (TIMER_GET (___timer0) < LIGHTS_FADE_IN_TIME)
        {
        
        }
*/
    }

void Lights::update (int throttle)
    {
    bool isBraking = 
        (throttle < THR_MID - THR_DELTA_TO_MIN*VESC_DEADBAND);



    switch (lightsMode)
        {
        case Lights::mode::_off:
            fl_lt = light_type::off;
            rl_lt = isBraking? 
                              light_type::full :
                              light_type::off; 
            break;
        case Lights::mode::_rear:
            break;
        case Lights::mode::_all:
            break;
        case Lights::mode::_auto:
            break;
        default:
            break;
        }
        
    }
