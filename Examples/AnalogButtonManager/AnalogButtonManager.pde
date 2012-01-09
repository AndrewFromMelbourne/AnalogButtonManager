#include "AnalogButtonManager.h"

//-------------------------------------------------------------------------

class SerialButton
:
    public AnalogButton
{
public:

    enum Buttons { RIGHT, UP, DOWN, LEFT, SELECT };

    SerialButton(
        int id,
        int low,
        int high,
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

AnalogButtonManager<5> analogButtonManager(A0, 30);

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