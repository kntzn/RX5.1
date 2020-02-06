#pragma once
#include "Config.h"

enum mode
    {
    lock,
    neutral,
    eco,
    normal,
    sport
    };


#define VESC_DEADBAND 0.15

// HW cfg:

#define WHEEL_DIA 0.083

// Modes config:

// ECO
#define BRAKE_MAX_ECO 0.25
#define POWER_MAX_ECO 0.25

// NORMAL
#define BRAKE_MAX_NRM 0.5
#define POWER_MAX_NRM 0.5

// SPORT
#define BRAKE_MAX_SPR 1.0
#define POWER_MAX_SPR 1.0
