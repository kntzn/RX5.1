// BMS.h

#ifndef _BMS_h
#define _BMS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "VoltageCurve.h"

class BMS
{
 protected:

	 uint8_t readPin;
 public:
	 BMS(uint8_t ADCpin);


};

#endif

