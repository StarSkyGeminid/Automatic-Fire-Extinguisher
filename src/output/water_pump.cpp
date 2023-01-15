#include "water_pump.h"
#include <Arduino.h>

void WaterPump::begin()
{
    pinMode(pin, OUTPUT);
    off();
}

void WaterPump::on()
{
    digitalWrite(pin, LOW);
    state = true;
}

void WaterPump::off()
{
    digitalWrite(pin, HIGH);
    state = false;
}

bool WaterPump::getState()
{
    return state;
}