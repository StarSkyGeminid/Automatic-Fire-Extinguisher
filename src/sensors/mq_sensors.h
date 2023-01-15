#ifndef __MQ_SENSORS_H__
#define __MQ_SENSORS_H__

#include <Arduino.h>

#include "sensor_config.h"
// #include <MQUnifiedsensor.h>

class MQSensors
{
private:
    // MQUnifiedsensor *sensor;
    uint8_t pin;

public:
    MQSensors(uint8_t sensorPin) : pin(sensorPin){};
    void begin();
    void calibrate();
    float getAirQuality();
};

#endif