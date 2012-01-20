#include "AnalogButtonManager.h"

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
