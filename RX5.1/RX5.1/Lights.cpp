// 
// 
// 

#include "Lights.h"
#include "Config.h"
#include "SysConfig.h"
#include "kstd.h"

void Lights::updateBrightness ()
    {
    int photoresistor_brightness = analogRead (pr_pin);

    switch (brightness)
        {
        case Lights::env_brightness::day:
            if (photoresistor_brightness < DUSK__BOTTOM_THRESH)
                brightness = env_brightness::dusk;
            if (photoresistor_brightness < NIGHT_BOTTOM_THRESH)
                brightness = env_brightness::night;
            break;
        case Lights::env_brightness::dusk:
            if (photoresistor_brightness > DAY______TOP_THRESH)
                brightness = env_brightness::day;
            if (photoresistor_brightness < NIGHT_BOTTOM_THRESH)
                brightness = env_brightness::night;
            break;
        case Lights::env_brightness::night:
            if (photoresistor_brightness > DUSK_____TOP_THRESH)
                brightness = env_brightness::dusk;
            if (photoresistor_brightness > DAY______TOP_THRESH)
                brightness = env_brightness::day;
            break;
        default:
            break;
        }
    }

void Lights::automaticLightType ()
    {
    switch (brightness)
        {
        case Lights::env_brightness::day:
            fl_lt = rl_lt = light_type::off;
            break;
        case Lights::env_brightness::dusk:
            fl_lt = rl_lt = light_type::half;
            break;
        case Lights::env_brightness::night:
            fl_lt = light_type::full;
            rl_lt = light_type::blink;
            break;
        default:
            break;
        }
    }

Lights::Lights (uint8_t front_lights_pin,
                uint8_t rear_lights_pin,
                uint8_t under_lights_pin,
                uint8_t photores_pin,
                mode lights_mode):
    fl_pin (front_lights_pin),
    rl_pin (rear_lights_pin),
    ul_pin (under_lights_pin),
    pr_pin (photores_pin),
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
    // Reads light sensor value and 
    // changes brightness parameter accordingly
    updateBrightness ();

    // Changes light type according to the mode
    switch (lightsMode)
        {
        case Lights::mode::_off:
            fl_lt = rl_lt = light_type::off; 
            break;
        case Lights::mode::_rear:
            fl_lt = light_type::off;
            rl_lt = light_type::half;
            break;
        case Lights::mode::_all:
            fl_lt = light_type::full;
            rl_lt = light_type::half;
            break;
        case Lights::mode::_auto:

            break;
        default:
            break;
        }
     

    // Stop signal
    if (throttle < THR_MID - THR_DELTA_TO_MIN*VESC_DEADBAND)
        rl_lt = light_type::full;
    }
