#pragma once

// describes the pinout of the ATMega328p uC

#define HC12_SET     2

#define HALL         3
#define PPM          4
#define LIGHTS_FRONT 5
#define LIGHTS_BACK  6

#define BUZZER       7
#define GROUNDED_0   8
#define GROUNDED_1   9


#define SD_CS        10

// Pins 11 - 13
// SPI

#define SHUNT_POS    A0
#define SHUNT_NEG    A1
#define V_BAT        A2

#define THERM        A3

#define DISPLAY_SDA  A4
#define DISPLAY_SCL  A5