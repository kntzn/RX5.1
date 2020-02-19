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
// |  __    __  |        |      |        |         |        |     |
// | |FL|  |BL| |        |______| <----- |_|Motor|_|        |_____|
// |_|__|__|__|_|           ^                                  |
// 	                        |__________________________________|
//

#include "kstd.h"
#include "Lights.h"
#include "BMS.h"
#include "Pinout.h"
#include "Communication.h"



void initialize();

int main()
	{
	initialize();
	Communication HC12(HC12_SET, 0); // TODO: EEPROM load channel
    BMS battery (V_BAT);
    Lights lights (LIGHTS_FRONT, LIGHTS_BACK,
                   LIGHTS_UNDER, LIGHTS_SENS, Lights::mode::_off);

	forever
		{
		}
	}

void initialize()
	{
	// Inits the uc
	init();	
	}

