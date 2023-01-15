#include "mq_sensors.h"

void MQSensors::begin()
{
    pinMode(pin, INPUT);
}

float MQSensors::getAirQuality()
{
    return analogRead(MQ_PIN);
}