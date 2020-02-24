#pragma once

// FW cfg:
#define VESC_DEADBAND 0.15
#define THR_MAX       2000
#define THR_MID       1500
#define THR_MIN       1000

#define THR_DELTA_TO_MAX (THR_MAX - THR_MID)
#define THR_DELTA_TO_MIN (THR_MID - THR_MIN)
#define THR_BRAKE_START  (THR_MID - THR_DELTA_TO_MIN*VESC_DEADBAND)
#define THR_POWER_START  (THR_MID + THR_DELTA_TO_MAX*VESC_DEADBAND)

#define HC12_BAUD 9600
#define BENCHMODE_SECONDS 25

// HW cfg:
#define WHEEL_DIA 0.083
#define BAT_WH 222.0
#define BAT_ABS_MIN_CELL 3.0

// Lights cfg
enum class lights_mode
    {
    _off,
    _rear,
    _all,
    _auto
    };

// Modes cfg:
enum class mode
    {
    lock,
    hybrid,
    eco,
    cruise,
    sport
    };

// LOCK
#define SPEED_MAX_LCK 2.0

// HYBRID
#define BRAKE_MAX_HYB (THR_MID - 275)
#define POWER_MAX_HYB (THR_MID + 117)
#define SPEED_MIN_HYB 5.00
#define SPEED_MID_HYB 15.0
#define SPEED_MAX_HYB 25.0

// ECO
#define BRAKE_MAX_ECO (THR_MID - 200)
#define POWER_MAX_ECO (THR_MID + 200)
#define RAMP_UP_T_ECO 1.0
#define MAX_SPEED_ECO 15.0

// NORMAL
#define BRAKE_MAX_CRU (THR_MID - 300)
#define POWER_MAX_CRU (THR_MID + 300)
#define RAMP_UP_T_CRU 0.50
#define MAX_SPEED_CRU 25.0

// SPORT
#define BRAKE_MAX_SPT (THR_MIN)
#define POWER_MAX_SPT (THR_MAX)
#define RAMP_UP_T_SPT 0.0
#define MAX_SPEED_SPT 100


#define CRUISE_SPEED_CUTOFF_START  2.0
#define CRUISE_SPEED_CUTOFF_END   -0.0


