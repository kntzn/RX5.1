// 
// 
// 

#include "BMS.h"
#include "Config.h"
#include "SysConfig.h"

double BMS::aver_analog (uint32_t times)
    {
    double result = 0.0;

    for (int i = 0; i < times; i++)
        result += analogRead (readPin);

    return result / static_cast<double> (times);
    }

long BMS::readVcc ()
    {
    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        ADMUX = _BV (REFS0) | _BV (MUX4) | _BV (MUX3) | _BV (MUX2) | _BV (MUX1);
    #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        ADMUX = _BV (MUX5) | _BV (MUX0);
    #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
        ADMUX = _BV (MUX3) | _BV (MUX2);
    #else
        ADMUX = _BV (REFS0) | _BV (MUX3) | _BV (MUX2) | _BV (MUX1);
    #endif
    delay (2); // Waiting for reference voltage to settle
    ADCSRA |= _BV (ADSC); // Start conversion
    while (bit_is_set (ADCSRA, ADSC)); // Measuring
    uint8_t low = ADCL; // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both
    long result = (high << 8) | low;

    result = REF_VOLTAGE * 1023 * 1000 / result;
    return result;
    }


void BMS::readVoltage ()
    {
    // Reads new voltage
    double new_bat_voltage = (readVcc () * aver_analog () / 1023 / 1000.0);

    // Gets voltage per cell according to the divider k
    new_bat_voltage *= ((R1_R2_DIVIDER / R1_DIVIDER) / DIVIDER_K) / N_CELLS;

    // Smootes out voltage value
    bat_voltage = new_bat_voltage * (1.0 - VOLTAGE_SMOOTH_K) + 
                      bat_voltage *        VOLTAGE_SMOOTH_K;
    }

void BMS::getPercentsFromVoltage ()
    {
    uint16_t left = 0, right = N_CURVE_VALUES - 1;

    while (right - left > 1)
        {
        uint16_t mid = (left + right) / 2;
        if (curve [(left + right) / 2] [0] < bat_voltage)
            right = mid;
        else
            left = mid;
        }

    percents = curve [left] [1] -
              (curve [left] [1] - curve [right] [1])*(curve [left] [0] - bat_voltage)/
                                  (curve [left] [0] - curve [right] [0]);
    }

void BMS::getUsageFromPercents ()
    {
    whDrawn = BAT_WH * (1.0 - percents/100.0);
    }

BMS::BMS (uint8_t ADCpin):
    readPin (ADCpin),
    bat_voltage (3.6),
    percents (0),
    whDrawn (0)
    {
    }

void BMS::update (int throttle)
    {
    // updates the voltage if battery is in the rest state
    if (throttle < THR_MID + THR_DELTA_TO_MAX*VESC_DEADBAND &&
        throttle > THR_MID - THR_DELTA_TO_MIN*VESC_DEADBAND)
        {
        readVoltage ();
        getPercentsFromVoltage ();
        getUsageFromPercents ();
        }
    }

double BMS::getCellVoltage ()
    {
    return bat_voltage;
    }

double BMS::getBatVoltage ()
    {
    return bat_voltage*N_CELLS;
    }

double BMS::getPercents ()
    {
    return percents;
    }

double BMS::getWhDrawn ()
    {
    return whDrawn;
    }