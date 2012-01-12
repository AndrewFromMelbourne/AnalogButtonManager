#ifndef ANALOG_BUTTON_H
#define ANALOG_BUTTON_H

//-------------------------------------------------------------------------
// Copyright (C) 2012 Andrew Duncan.
//
// This software may be distributed and modified under the terms of the GNU
// General Public License version 2 (GPL2) as published by the Free Software
// Foundation and appearing in the file GPL2.TXT included in the packaging
// of this file. Please note that GPL2 Section 2[b] requires that all works
// based on this software must also be made publicly available under the
// terms of the GPL2 ("Copyleft").
//
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
