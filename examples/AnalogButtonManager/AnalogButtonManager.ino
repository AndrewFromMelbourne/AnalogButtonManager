#include "AnalogButtonManager.h"

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

class SerialButton
:
    public AnalogButton
{
public:

    enum Buttons { RIGHT, UP, DOWN, LEFT, SELECT };

    SerialButton(
        uint8_t id,
        uint16_t low,
        uint16_t high,
        AnalogButtonManagerInterface& abmi,
        const char* name)
    :
        AnalogButton(id, low, high, abmi),
        _name(name)
    {
    }
    
    virtual void pressed();
    virtual void repeated();
    virtual void released();
    
private:

    const char* _name;
};

//-------------------------------------------------------------------------

void SerialButton::pressed()
{
    Serial.print("button ");
    Serial.print(_name);
    Serial.println(" was pressed");
}

//-------------------------------------------------------------------------

void SerialButton::repeated()
{
    Serial.print("button ");
    Serial.print(_name);
    Serial.println(" repeated");
}

//-------------------------------------------------------------------------

void SerialButton::released()
{
    Serial.print("button ");
    Serial.print(_name);
    Serial.println(" was released");
}

//-------------------------------------------------------------------------

AnalogButtonManager<5> analogButtonManager(A0);

SerialButton b1(SerialButton::RIGHT,
                0,
                20,
                analogButtonManager,
                "right");

SerialButton b2(SerialButton::UP,
                135,
                155,
                analogButtonManager,
                "up");

SerialButton b3(SerialButton::DOWN,
                319,
                339,
                analogButtonManager,
                "down");

SerialButton b4(SerialButton::LEFT,
                495,
                515,
                analogButtonManager,
                "left");

SerialButton b5(SerialButton::SELECT,
                731,
                751,
                analogButtonManager,
                "select");

//-------------------------------------------------------------------------

void setup() 
{
    Serial.begin(115200); 
    Serial.println("Starting Analog Button Manager Test.");
}
 
//-------------------------------------------------------------------------

void loop() 
{  
    analogButtonManager.task();
}
