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

#include "BMS.h"
#include "Pinout.h"
#include "Communication.h"

#define forever for (;;)

void initialize();

int main()
	{
	initialize();
	Communication HC12(HC12_SET, 0); // TODO: EEPROM load channel
	

	forever
		{
		}
	}

void initialize()
	{
	// Inits the uc
	init();	
	}

