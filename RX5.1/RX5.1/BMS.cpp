// 
// 
// 

#include "BMS.h"
#include "Config.h"

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
    new_bat_voltage *= ((24300.0 / 4300.0) / DIVIDER_K) / N_CELLS;

    // Smootes out voltage value
    bat_voltage = new_bat_voltage * (1.0 - VOLTAGE_SMOOTH_K) + 
                      bat_voltage *        VOLTAGE_SMOOTH_K;
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
    if (!throttle)
        readVoltage ();

    //percents = getPrecentsFromVoltage ()

    }
