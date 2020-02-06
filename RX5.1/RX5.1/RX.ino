/*
 Name:		RX.ino
 Created:	8/6/2019 12:13:15 AM
 Author:	kntzn
*/

// TODO:
// move last_avail var to the Comm. class



#include "HallSensor.h"
#include "Comm.h"
#include "Pinout.h"
#include "SysConfig.h"

#include <TM1637Display.h>
#include "Battery.h"
#include <Servo.h>

void initialize ()
    { 
    // inits the uC
    init ();

    pinMode (HC12_SET,      OUTPUT);
    digitalWrite (HC12_SET, HIGH);

    pinMode (HALL,          INPUT);
    pinMode (PPM,           OUTPUT);
    
    // Lights
    pinMode (LIGHTS_FRONT,  OUTPUT);
    analogWrite (LIGHTS_FRONT,   0);
    pinMode (LIGHTS_BACK,   OUTPUT);
    analogWrite (LIGHTS_BACK,    0);
    
    pinMode (BUZZER,        OUTPUT);
    
    // Unused pins
    pinMode (GROUNDED_0,    INPUT);
    pinMode (GROUNDED_1,    INPUT);

    pinMode (SD_CS,         OUTPUT);

    // XTL Wattmeter
    pinMode (SHUNT_POS,     INPUT);
    pinMode (SHUNT_NEG,     INPUT);
    pinMode (V_BAT,         INPUT);

    pinMode (THERM,         INPUT);

    // Display 
    pinMode (DISPLAY_SDA,  OUTPUT);
    pinMode (DISPLAY_SCL,  OUTPUT);
    }

// TODO: move to the class
uint16_t convertToPPMvalue (uint16_t thr, uint16_t prev_thr, mode current_mode);

int main ()
    {
    initialize ();

    attachInterrupt (1, hsInterruptHandler, FALLING);

    TM1637Display disp (DISPLAY_SCL, DISPLAY_SDA);
    disp.setBrightness (7);

    Battery battety (V_BAT);
    battety.batMeasure ();
    unsigned long last_bat_upd = millis ();

    Servo VESC;
    VESC.attach (PPM);
    VESC.writeMicroseconds (PPM_MID);
    unsigned long last_avail = millis ();

    int last_reading = PPM_MID;
    mode current_mode = mode::normal;

    Serial.begin (9600);

    // Beeps 6 times at startup
    for (int i = 0; i < 6; i++)
        {
        digitalWrite (BUZZER, HIGH);
        delay (100);
        digitalWrite (BUZZER, LOW);
        delay (200);
        }

    Communication HC12;
    
    // Main cycle
    while (true)
        {
        // If single-byte one-way communication is active
        if (HC12.rawinputActive ())
            { 
            while (Serial.available ())
                last_reading =
                    convertToPPMvalue (static_cast <uint16_t> (Serial.read ()) * 4,
                                       last_reading, mode::sport);
                
            VESC.writeMicroseconds (last_reading);

            last_avail = millis ();
            }
        // else using default two-way communication
        else
            { 
            // If packet available
            Communication::command req;
            if ((req = HC12.receiveRequest ()) !=
                Communication::command::nocmd)
                {
                switch (req)
                    {
                    case Communication::command::nocmd:
                        break;
                    case Communication::command::throttle:
                        {
                        uint16_t thr = HC12.argbuf () [0] * 256 +
                            HC12.argbuf () [1];

                        last_reading = convertToPPMvalue (thr, last_reading, current_mode);

                        VESC.writeMicroseconds (last_reading);

                        last_avail = millis ();

                        break;
                        }
                    case Communication::command::mode:
                        {
                        current_mode = static_cast <mode> (HC12.argbuf () [0]);

                        break;
                        }
                    case Communication::command::voltage:
                        // Response
                        HC12.sendResponse (Communication::response::voltage,
                                           battety.getBatVoltage () * 1000);
                        break;
                    case Communication::command::speed:
                        // Response
                        HC12.sendResponse (Communication::response::speed,
                                           getSpeed () * 1000);
                        break;

                    case Communication::command::raw:
                        // Activates rawinput communication mode
                        if (HC12.argbuf ()[0] == 
                            static_cast <char> (Communication::command::raw) &&
                            HC12.argbuf () [1] ==
                            static_cast <char> (Communication::command::raw))
                                HC12.activateRawinput ();
                        break;
                    default:
                        // Command discarded
                        break;
                    }

                }
            }
        
        // Battery voltage update timer
        if (millis () - last_bat_upd > 500)
            {
            battety.batMeasure ();
            disp.showNumberDec (int (battety.getBatVoltage () * 10.0) * 10);
            last_bat_upd += 500;
            }

        // Failsafe handler 
        // Also beeps for FAILSAFE_RAMP_UP ms when diconnects
        if (!failSafe (VESC, last_reading, last_avail))
            {
            digitalWrite (BUZZER, LOW);
            VESC.writeMicroseconds (last_reading);
            }
        }
    }

// TO DO TO DO TODO TODO TODO TODO TODOOOOOO TODODODODO: move to the class
// param: thr          - new 10-bit throttle value
// param: prev_thr     - previous 10-bit throttle value
// param: current_mode - selected riding style    
uint16_t convertToPPMvalue (uint16_t thr, uint16_t prev_thr, mode current_mode)
    { 
    // TODO: latency for slow modes

    // TODO: constant deadband
    // TODO: strong brakes at moderate/fast modes

    switch (current_mode)
        {
        case mode::lock:
            return PPM_MIN;
            break;
        case mode::neutral:
            return PPM_MID;
            break;
        case mode::eco:
            return map (thr, 0, 1023, 1000, 2000);
            break;
        case mode::normal:
            return map (thr, 0, 1023, 1000, 2000);
            break;
        case mode::sport:
            return map (thr, 0, 1023, 1000, 2000);
            break;
        default:
            break;
        }

    return PPM_MID;
    }

// TODO: move to the class
bool failSafe (Servo &VESC, uint16_t last_reading, unsigned long int last_avail)
    { 
    if (millis () - last_avail > FAILSAFE_MS)
        {
        unsigned long delta = (millis () - last_avail) - FAILSAFE_MS;

        if (delta < FAILSAFE_RAMP_UP)
            {
            float k = float (delta) / float (FAILSAFE_RAMP_UP);

            int toVESC = k * PPM_MIN +
                (1.f - k) * (last_reading);

            VESC.writeMicroseconds (toVESC);

            digitalWrite (BUZZER, HIGH);
            }
        else
            {
            VESC.writeMicroseconds (PPM_MIN);

            digitalWrite (BUZZER, LOW);
            }

        }

    return (millis () - last_avail > FAILSAFE_MS);
    }