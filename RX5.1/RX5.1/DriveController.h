// DriveController.h

#ifndef _DRIVECONTROLLER_h
#define _DRIVECONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "MotorController.h"
#include "HallSensor.h"
#include "Sarray.h"

#define D_C_UNREADY -1

class DriveController
    {
    private:
        struct BenchParams
            {
            float V, A, U;
            };
        
        uint16_t tripId, benchId;
        double speed, acceleration;
        double odo, trip, totalTrip, left;
        double consumption, batteryLeft;

        HallSensor      hallSens;
        MotorController motor;
        //sarray <BenchParams, BENCHMODE_SECONDS> benchmarkBuffer;
        


    public:
        DriveController (uint16_t tripId, uint16_t benchId,
                         uint8_t motor_pin, mode drive_mode,
                         double battery_wh_left,
                         double odometer, double previos_trip = 0.0);

        void update (int new_throttle, mode new_mode, BMS battery);



        // Returns output to the motor
        int getPPMoutput ();

        // Returns odo if board has just stopped
        // Otherwise returns UNREADY flag
        int odoToWriteToEEPROM ();
        // Returns totalTrip if board has just stopped
        // Otherwise returns UNREADY flag
        int trpToWriteToEEPROM ();
        // Returns mode if board has just stopped
        // Otherwise returns UNREADY flag
        int trpToWriteToEEPROM ();
    };

#endif

