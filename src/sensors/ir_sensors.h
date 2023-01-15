#ifndef __IR_SENSORS_H__
#define __IR_SENSORS_H__

#include <Arduino.h>

class IRSensors
{
private:
    int irValue;
    uint8_t pin;

public:
    IRSensors(uint8_t pin)
    {
        this->pin = pin;
        pinMode(pin, INPUT_PULLUP);
        digitalWrite(pin, LOW);
    };
    int read()
    {
        irValue = analogRead(pin);
        return irValue;
    };
};

#endif