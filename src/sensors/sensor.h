#ifndef __SENSORS_H__
#define __SENSORS_H__

#include "Arduino.h"
#include <DHT.h>

#include "sensor_config.h"
#include "mq_sensors.h"
#include "ir_sensors.h"

struct SensorData
{
    bool updated;
    float temperature;
    float humidity;
    int airQuality;
    int irValue; 
};

class Sensor
{
private:
    DHT *dht;
    MQSensors *mq;
    IRSensors *ir;
    SensorData sensorData;

public:
    ~Sensor();

    void init();
    float getTemperature();
    float getHumidity();
    int getAirQuality();
    int getIrValue();
    SensorData getSensorData();
};

#endif