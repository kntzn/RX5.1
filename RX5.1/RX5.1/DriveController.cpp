// 
// 
// 

#include "DriveController.h"

DriveController::DriveController (uint16_t trip_id, uint16_t bench_id,
                                  uint8_t motor_pin, mode drive_mode,
                                  double battery_wh_left,
                                  double odometer, double previos_trip):
    motor (motor_pin, drive_mode),
    hallSens (HALL),
    tripId (trip_id), benchId (bench_id),
    speed (0.0), acceleration (0.0),
    odo (odometer), totalTrip (previos_trip), trip (0.0),
    consumption (0), batteryLeft (battery_wh_left)
    {
    }

void DriveController::update (uint8_t* data_buffer, BMS battery)
    {
    // Updates components
    if (data_buffer)
        {
        // gets two-bit throttle value
        int thr = data_buffer [0] * 256 + data_buffer [1];
        motor.update (thr, 
                      static_cast <mode> (data_buffer [2]), 
                      hallSens.getSpeed ());
        }
    // If no packet available - turn on failsafe
    else
        motor.update (0, mode::eco, hallSens.getSpeed ());
        
    hallSens.update ();
    
    // Speed and accel
    speed        = hallSens.getSpeed ();
    acceleration = hallSens.getAccel ();

    // Distance
    double dS = hallSens.getNewTurnsAmount () * (WHEEL_DIA*PI);
    odo += dS;
    trip += dS;
    totalTrip += dS;
    
    // battery consumption as Wh/km scince startup
    consumption = trip?
                  (batteryLeft - battery.getWhLeft ()) / (trip) : 
                  0.0;
    left = consumption?
           battery.getWhLeft () / consumption:
           0.0;

    }

int DriveController::getPPMoutput ()
    {
    return motor.getThrottleOutput ();
    }

int DriveController::odoToWriteToEEPROM ()
    {
    return (speed != 0.0 && hallSens.getSpeed () == 0.0)?
            odo, speed = hallSens.getSpeed ():
            D_C_UNREADY;
    }

int DriveController::trpToWriteToEEPROM ()
    {
    return (speed != 0.0 && hallSens.getSpeed () == 0.0)?
            totalTrip, speed = hallSens.getSpeed ():
            D_C_UNREADY;
    }
