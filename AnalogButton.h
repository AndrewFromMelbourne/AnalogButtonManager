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

#ifndef ANALOG_BUTTON_H
#define ANALOG_BUTTON_H

//-------------------------------------------------------------------------

#include <inttypes.h>

//-------------------------------------------------------------------------

class AnalogButtonManagerInterface;

//-------------------------------------------------------------------------

class AnalogButton 
{
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

private: 

    uint8_t _id;
    uint16_t _low;
    uint16_t _high;     
};

//-------------------------------------------------------------------------

#endif
