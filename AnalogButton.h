#ifndef ANALOG_BUTTON_H
#define ANALOG_BUTTON_H

//-------------------------------------------------------------------------

#include <inttypes.h>

//-------------------------------------------------------------------------

class AnalogButtonManagerInterface;

//-------------------------------------------------------------------------

class AnalogButton 
{
private: 

	uint8_t _id;
	uint16_t _low;
	uint16_t _high;     

public:  

	AnalogButton(
		uint8_t id,
		uint16_t low,
		uint16_t high,
		AnalogButtonManagerInterface& abmi);

	virtual void pressed() {}
	virtual void repeated() {}
	virtual void released() {}

	uint8_t getId() const { return _id; }
	uint16_t getLow() const { return _low; }
	uint16_t getHigh() const { return _high; }

};

//-------------------------------------------------------------------------

#endif
