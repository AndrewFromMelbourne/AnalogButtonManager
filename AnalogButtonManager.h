#ifndef ANALOG_BUTTON_MANAGER_H
#define ANALOG_BUTTON_MANAGER_H

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

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <inttypes.h>

#include "AnalogButton.h"

//-------------------------------------------------------------------------

class AnalogButtonManagerInterface
{
public:

	virtual void manageButton(AnalogButton* ab) {}
};

//-------------------------------------------------------------------------

template<uint8_t NumberOfButtons>
class AnalogButtonManager
:
	public AnalogButtonManagerInterface
{
public:

	//---------------------------------------------------------------------

	AnalogButtonManager(
		uint8_t analogPin,
		uint16_t debounceDelay = 100,
		uint16_t repeatDelay = 1000,
		uint8_t samplingInterval = 20)
	:
		_analogPin(analogPin),
		_debounceDelay(debounceDelay),
		_lastDebounceTime(0),
		_repeatDelay(repeatDelay),
		_lastRepeatTime(0),
	    _previousReadTime(0),
	    _samplingInterval(samplingInterval),
		_activeButton(0),
		_pressedButton(0),
		_buttonsManaged(0)
	{
		pinMode(analogPin, INPUT);
		digitalWrite(analogPin, LOW);
	}
  
	//---------------------------------------------------------------------

	virtual void
	manageButton(
		AnalogButton* ab)
	{
		if (_buttonsManaged < NumberOfButtons)
		{
			_buttons[_buttonsManaged++] = ab;
		}
	}

	//---------------------------------------------------------------------

	void
	task()
	{
		uint32_t now = millis();

	    if ((now - _previousReadTime) < _samplingInterval)
		{
			return;
		}

		uint16_t value = analogRead(_analogPin);   
		_previousReadTime = now;

		boolean buttonFound = false;

		for (
			int8_t i = 0;
			(i < _buttonsManaged) && (not buttonFound);
			++i) 
		{        
			if (value <= _buttons[i]->getHigh()
				&& value >= _buttons[i]->getLow()) 
			{      
				buttonFound = true;

				if (_activeButton != _buttons[i])
				{
					_lastDebounceTime = now;
					_lastRepeatTime = 0;
					_activeButton = _buttons[i];

					if (_pressedButton)
					{
						_pressedButton->released();
						_pressedButton = 0;
					}
				}
				else if ((now - _lastDebounceTime) > _debounceDelay)
				{
					if (not _pressedButton)
					{
						_activeButton->pressed();
						_pressedButton = _activeButton;

						_lastDebounceTime = now;
						_lastRepeatTime = _lastDebounceTime;
					}
					else if ((now - _lastRepeatTime) > _repeatDelay)
					{
						_pressedButton->repeated();

						_lastDebounceTime = now;
						_lastRepeatTime = _lastDebounceTime;
					}
				}
			} 
		}

		if (not buttonFound)
		{
			_activeButton = 0;

			if (_pressedButton)
			{
				_pressedButton->released();
				_pressedButton = 0;
			}
		}
	}

	//---------------------------------------------------------------------

private:

	uint8_t _analogPin;
  
	uint16_t _debounceDelay;
	uint32_t _lastDebounceTime;

	uint16_t _repeatDelay;
	uint32_t _lastRepeatTime;

	uint32_t _previousReadTime;
	uint8_t _samplingInterval;
  
	AnalogButton* _activeButton;
	AnalogButton* _pressedButton;

	int8_t _buttonsManaged;
	AnalogButton* _buttons[NumberOfButtons];
};

//-------------------------------------------------------------------------

#endif
