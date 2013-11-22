//-------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Copyright (c) 2013 Andrew Duncan
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------

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

    static const uint16_t defaultDebounceDelay = 40;
    static const uint16_t defaultRepeatDelay = 1000;
    static const uint8_t defaultSamplingInterval = 20;

    //---------------------------------------------------------------------

    AnalogButtonManager(
        uint8_t analogPin)
    :
        _analogPin(analogPin),
        _debounceDelay(defaultDebounceDelay),
        _debounceCountDown(0),
        _repeatDelay(defaultRepeatDelay),
        _repeatCountDown(0),
        _previousReadTime(millis()),
        _samplingInterval(defaultSamplingInterval),
        _minButtonsValue(0),
        _maxButtonsValue(0),
        _activeButton(0),
        _pressedButton(0),
        _buttonsManaged(0)
    {
        pinMode(analogPin, INPUT);
        digitalWrite(analogPin, LOW);
    }
  
    //---------------------------------------------------------------------

    uint16_t
    getDebounceDelay() const { return _debounceDelay; }

    void
    setDebounceDelay(
        uint16_t debounceDelay)
    {
        _debounceDelay = debounceDelay;
    }

    uint16_t
    getRepeatDelay() const { return _repeatDelay; }

    void
    setRepeatDelay(
        uint16_t repeatDelay)
    {
        _repeatDelay = repeatDelay;
    }

    uint8_t
    getSamplingInterval() const { return _samplingInterval; }

    void
    setSamplingInterval(
        uint8_t samplingInterval)
    {
        _samplingInterval = samplingInterval;
    }

    //---------------------------------------------------------------------

    virtual void
    manageButton(
        AnalogButton* ab)
    {
        if (_buttonsManaged < NumberOfButtons)
        {
            if (_buttonsManaged == 0)
            {
                _minButtonsValue = ab->getLow();
                _maxButtonsValue = ab->getHigh();
            }
            else
            {
                if (_minButtonsValue > ab->getLow())
                {
                    _minButtonsValue = ab->getLow();
                }

                if (_maxButtonsValue < ab->getHigh())
                {
                    _maxButtonsValue = ab->getHigh();
                }
            }

            _buttons[_buttonsManaged++] = ab;
        }
    }

    //---------------------------------------------------------------------

    void
    task()
    {
        uint32_t now = millis();
        uint32_t elapsed = now - _previousReadTime;

        if (_previousReadTime > now)
        {
            elapsed = (0xFFFFFFFF - _previousReadTime) + now + 1;
        }

        if (elapsed < _samplingInterval)
        {
            return;
        }

        //-----------------------------------------------------------------

        if (_debounceCountDown > 0)
        {
            if (_debounceCountDown < elapsed)
            {
                _debounceCountDown = 0;
            }
            else
            {
                _debounceCountDown -= elapsed;
            }
        }

        if (_repeatCountDown > 0)
        {
            if (_repeatCountDown < elapsed)
            {
                _repeatCountDown = 0;
            }
            else
            {
                _repeatCountDown -= elapsed;
            }
        }

        //-----------------------------------------------------------------

        uint16_t value = analogRead(_analogPin);   
        _previousReadTime = now;

        if ((value < _minButtonsValue) || (value > _maxButtonsValue))
        {
            _activeButton = 0;

            if (_pressedButton)
            {
                _pressedButton->released();
                _pressedButton = 0;
            }

            return;
        }

        //-----------------------------------------------------------------

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
                    _debounceCountDown = _debounceDelay;
                    _activeButton = _buttons[i];

                    if (_pressedButton)
                    {
                        _pressedButton->released();
                        _pressedButton = 0;
                    }
                }
                else if ((not _pressedButton) && (_debounceCountDown == 0))
                {
                    _activeButton->pressed();
                    _pressedButton = _activeButton;

                    _repeatCountDown = _repeatDelay;
                }
                else if (_pressedButton && (_repeatCountDown == 0))
                {
                    _pressedButton->repeated();

                    _debounceCountDown = _debounceDelay;
                    _repeatCountDown = _repeatDelay;
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
    uint32_t _debounceCountDown;

    uint16_t _repeatDelay;
    uint32_t _repeatCountDown;

    uint32_t _previousReadTime;
    uint8_t _samplingInterval;

    uint16_t _minButtonsValue; 
    uint16_t _maxButtonsValue;
  
    AnalogButton* _activeButton;
    AnalogButton* _pressedButton;

    int8_t _buttonsManaged;
    AnalogButton* _buttons[NumberOfButtons];
};

//-------------------------------------------------------------------------

#endif
