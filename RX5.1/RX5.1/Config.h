#pragma once

#define PPM_MIN 1000
#define PPM_MID 1500
#define PPM_MAX 2000

#define FAILSAFE_RAMP_UP 4000
#define FAILSAFE_MS      500

#define DIVIDER_K        0.956826
#define REF_VOLTAGE      1.1
#define VOLTAGE_SMOOTH_K 0.99
#define N_CELLS          6
#define BAT_ABS_MIN_CELL 3.0
#define BAT_WH           222
#define R1_DIVIDER       4300
#define R1_R2_DIVIDER    24300

#define LIGHTS_FADE_IN_TIME 2000
#define LIGHTS_BLINK_PERIOD 500
#define LIGHTS_SMOOTH_K     0.95
#define BRIGHTNESS_FULL     255
#define BRIGHTNESS_HALF     127
#define BRIGHTNESS_OFF      0
#define NIGHT_BOTTOM_THRESH 250
#define DUSK__BOTTOM_THRESH 475
#define DUSK_____TOP_THRESH 525
#define DAY______TOP_THRESH 630