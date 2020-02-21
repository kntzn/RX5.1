// 
// 
// 

#include "DriveController.h"

DriveController::DriveController (uint16_t trip_id, uint16_t bench_id,
                                  uint8_t motor_pin, mode drive_mode,
                                  double odometer, double previos_trip):
    motor (motor_pin, drive_mode),
    hallSens (HALL),
    tripId (trip_id), benchId (bench_id),
    speed (0.0), acceleration (0.0),
    odo  (odometer), trip (previos_trip)
    {
    }
