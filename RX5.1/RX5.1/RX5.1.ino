/*
 Name:		RX5.ino
 Created:	2/6/2020 9:01:47 PM
 Author:	kntzn
*/ 

//                                          ____
//       --------------------------------> | SD | <------------
// 	    |                                  |____|              |
//      |                                     ^                |
//  ____|_______          ______          ____|____          __|__
// |   Lights   | <----- | Comm | -----> |  Ride   | <----- | BMS |
// |  __ D  __  |        |      |        |         |        |  D  |
// | |FL|  |BL| |        |______| <----- |_|Motor|_|        |_____|
// |_|__|__|__|_|           ^                                  |
// 	                        |__________________________________|
//


#include "DriveController.h"
#include "kstd.h"
#include "Lights.h"
#include "BMS.h"
#include "Pinout.h"
#include "Communication.h"


void initialize();

int main()
	{
	initialize();
    Serial.begin (HC12_BAUD);
    while (!Serial);
	Communication HC12(HC12_SET, 0); // TODO: EEPROM load channel
    BMS battery (V_BAT);
    battery.update (THR_MID);

    Lights lights (LIGHTS_FRONT, LIGHTS_BACK,
                   LIGHTS_UNDER, LIGHTS_SENS, lights_mode::_off);
    
    DriveController drive (0, 0, PPM, mode::sport, HALL, battery.getWhLeft (), 0.0, 0.0);
    
    // TODO: EEPROM load mode, 
    // tripId, benchId, pin,
    // mode, prev_tip+cont.last.trip

    //lights.setUnderLights (true);

    // Temp. variables
    bool failsafe = false;
    int throttle = THR_MID;
    mode last_mode = mode::eco;

    // TODO:
    // while (HC12.receiveRequest () != motor);
    // if (cont_bit == 1)
    //    eeprom
    // else
    //    new

    forever
        {
        failsafe = true;
        
        switch (HC12.receiveRequest ())
            {
            case Communication::command::nocmd:
                break;
            case Communication::command::changeCh:
                HC12.changeCh (HC12.argbuf ()[0]);
                break;
            case Communication::command::motor:
                drive.update (HC12.argbuf (), battery);
                failsafe = false;
                break;
            case Communication::command::lights:
                lights.setValues (HC12.argbuf ());
                break;
            case Communication::command::trip:
                drive.readTripInfo (HC12.argbuf ());
                HC12.sendResponse (Communication::response::trip);
                break;
            case Communication::command::battery:
                battery.readBatteryInfo (HC12.argbuf ());
                HC12.sendResponse (Communication::response::battery);
                break;
            }

        // Modules update block
        if (failsafe)
            drive.update (nullptr, battery);
        battery.update (drive.getPPMoutput ());
        lights.update  (drive.getPPMoutput ());
		}
	}
   
void initialize()
	{
	// Inits the uc
	init();	

    pinMode (HALL, INPUT);
    pinMode (BUZZER, OUTPUT);
    digitalWrite (BUZZER, LOW);

    pinMode (SD_CS, OUTPUT);
    digitalWrite (SD_CS, HIGH);
	}

