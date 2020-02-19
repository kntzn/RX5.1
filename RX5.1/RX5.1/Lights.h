// Lights.h

#ifndef _LIGHTS_h
#define _LIGHTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Lights
    {
    public:
        enum class mode
            {
            _off,
            _rear,
            _all,
            _auto
            };
    private:
        enum class light_type
            {
            off,
            blink,
            half,
            full
            };
        enum class env_brightness
            {
            day,
            dusk,
            night
            };
        
        light_type fl_lt, rl_lt;
        mode lightsMode;
        env_brightness brightness;

        uint8_t fl_pin, rl_pin, ul_pin, pr_pin;

        void updateBrightness ();
        void automaticLightType ();

    public:
        Lights (uint8_t front_lights_pin,
                uint8_t rear_lights_pin,
                uint8_t under_lights_pin,
                uint8_t photores_pin,
                mode lights_mode);

        void update (int throttle);
        
        void setMode (mode new_mode);

    };

#endif

