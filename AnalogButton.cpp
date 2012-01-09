#include "AnalogButton.h"
#include "AnalogButtonManager.h"

AnalogButton:: AnalogButton(
	uint8_t id,
	uint16_t low,
	uint16_t high,
	AnalogButtonManagerInterface& abmi)
:
	_id(id),
	_low(low),
	_high(high)
{
	abmi.manageButton(this);
}
