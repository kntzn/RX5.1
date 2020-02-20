// 
// 
// 

#include "Lights.h"
#include "Config.h"
#include "SysConfig.h"
#include "Pinout.h"
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

void Lights::writeToLeds (bool brakes)
    {
    switch (fl_lt)
        {
        case Lights::light_type::full:
            fl_br = fl_br*(LIGHTS_SMOOTH_K) +BRIGHTNESS_FULL*(1.0 - LIGHTS_SMOOTH_K);
            break;
        case Lights::light_type::half:
            fl_br = fl_br*(LIGHTS_SMOOTH_K) + BRIGHTNESS_HALF*(1.0 - LIGHTS_SMOOTH_K);
            break;
        default:
            fl_br = fl_br*(LIGHTS_SMOOTH_K) +BRIGHTNESS_OFF*(1.0 - LIGHTS_SMOOTH_K);
            break;
        }

    switch (rl_lt)
        {
        case Lights::light_type::full:
            rl_br = rl_br*(LIGHTS_SMOOTH_K) +BRIGHTNESS_FULL*(1.0 - LIGHTS_SMOOTH_K);
            break;
        case Lights::light_type::half:
            rl_br = rl_br*(LIGHTS_SMOOTH_K) +BRIGHTNESS_HALF*(1.0 - LIGHTS_SMOOTH_K);
            break;
        case Lights::light_type::blink:
            rl_br = (millis ()%LIGHTS_BLINK_PERIOD > LIGHTS_BLINK_PERIOD/2) *
                    (BRIGHTNESS_FULL - BRIGHTNESS_OFF) +
                     BRIGHTNESS_OFF;
            break;
        default:
            rl_br = rl_br*(LIGHTS_SMOOTH_K) +BRIGHTNESS_OFF*(1.0 - LIGHTS_SMOOTH_K);
            break;
        }

    // Stop signal
    if (!brakes)
        analogWrite (LIGHTS_BACK,  rl_br);
    else 
        analogWrite (LIGHTS_BACK, BRIGHTNESS_FULL);

    analogWrite (LIGHTS_FRONT, fl_br);
    digitalWrite (LIGHTS_UNDER, ul);
    }

Lights::Lights (uint8_t front_lights_pin,
                uint8_t rear_lights_pin,
                uint8_t under_lights_pin,
                uint8_t photores_pin,
                mode lights_mode,
                bool under_ligths):
    fl_pin (front_lights_pin), rl_pin (rear_lights_pin),
    ul_pin (under_lights_pin), pr_pin (photores_pin),
    lightsMode (lights_mode), fl_br (0), rl_br (0), ul (under_ligths),
    brightness (env_brightness::day)
    {
    pinMode (front_lights_pin, OUTPUT);
    pinMode ( rear_lights_pin, OUTPUT);
    pinMode (under_lights_pin, OUTPUT);
    pinMode (    photores_pin, INPUT);

    update (THR_MID);
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
            automaticLightType ();
            break;
        default:
            break;
        }
    
    // Changes output to leds
    writeToLeds (throttle < THR_MID - THR_DELTA_TO_MIN*VESC_DEADBAND);
    }

void Lights::setMode (mode new_mode)
    {
    lightsMode = new_mode;
    }

void Lights::setUnderLights (bool on)
    {
    ul = on;
    }
