#pragma once
#include "Config.h"

// FW cfg:
#define VESC_DEADBAND 0.15
#define THR_MAX       2000
#define THR_MID       1500
#define THR_MIN       1000

#define THR_DELTA_TO_MAX (THR_MAX - THR_MID)
#define THR_DELTA_TO_MIN (THR_MID - THR_MIN)

// HW cfg:
#define WHEEL_DIA 0.083
#define BAT_WH 222

// Modes cfg:
enum class mode
    {
    lock,
    hybrid,
    eco,
    cruise,
    sport
    };

// ECO
#define BRAKE_MAX_ECO 0.25
#define POWER_MAX_ECO 0.25

// NORMAL
#define BRAKE_MAX_NRM 0.5
#define POWER_MAX_NRM 0.5

// SPORT
#define BRAKE_MAX_SPR 1.0
#define POWER_MAX_SPR 1.0
