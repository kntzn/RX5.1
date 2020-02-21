// DriveController.h

#ifndef _DRIVECONTROLLER_h
#define _DRIVECONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "MotorController.h"
#include "Sarray.h"

class DriveController
    {
    private:
        struct BenchParams
            {
            float V, A, U;
            };
        
        uint16_t tripId, benchId;
        double speed, acceleration;
        double odo, trip, left;
        double consumption;

        MotorController motor;
        sarray <BenchParams, BENCHMODE_SECONDS> benchmarkBuffer;

    public:

        DriveController (uint16_t tripId, uint16_t benchId,
                         uint8_t motor_pin, mode drive_mode,
                         double odometer, double previos_trip = 0.0);
    };

#endif

