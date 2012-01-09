#ifndef ANALOG_BUTTON_MANAGER_H
#define ANALOG_BUTTON_MANAGER_H

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
		uint16_t repeatDelay = 1000)
	:
		_analogPin(analogPin),
		_debounceDelay(debounceDelay),
		_lastDebounceTime(0),
		_repeatDelay(repeatDelay),
		_lastRepeatTime(0),
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
		uint16_t value = analogRead(_analogPin);   

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
					_lastDebounceTime = millis();
					_lastRepeatTime = 0;
					_activeButton = _buttons[i];

					if (_pressedButton)
					{
						_pressedButton->released();
						_pressedButton = 0;
					}
				}
				else if ((millis() - _lastDebounceTime) > _debounceDelay)
				{
					if (not _pressedButton)
					{
						_activeButton->pressed();
						_pressedButton = _activeButton;

						_lastDebounceTime = millis();
						_lastRepeatTime = _lastDebounceTime;
					}
					else if ((millis() - _lastRepeatTime) > _repeatDelay)
					{
						_pressedButton->repeated();

						_lastDebounceTime = millis();
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
  
	AnalogButton* _activeButton;
	AnalogButton* _pressedButton;

	int8_t _buttonsManaged;
	AnalogButton* _buttons[NumberOfButtons];
};

//-------------------------------------------------------------------------

#endif
