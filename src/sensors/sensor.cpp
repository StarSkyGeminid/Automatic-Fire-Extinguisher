#include "sensor.h"

Sensor::~Sensor()
{
    delete dht;
    delete mq;

    dht = NULL;
    mq = NULL;
}

void Sensor::init()
{
    dht = new DHT(DHT_SENSOR_PIN, DHT_TYPE);
    dht->begin();

    mq = new MQSensors(MQ_PIN);
    mq->begin();

    ir = new IRSensors(IR_PIN);
}

float Sensor::getTemperature()
{
    sensorData.temperature = dht->readTemperature();
    return sensorData.temperature;
}

float Sensor::getHumidity()
{
    sensorData.humidity = dht->readHumidity();
    return sensorData.humidity;
}
int Sensor::getAirQuality()
{
    sensorData.airQuality = mq->getAirQuality();
    return sensorData.airQuality;
}
int Sensor::getIrValue()
{
    sensorData.irValue = ir->read();
    return sensorData.irValue;
}
SensorData Sensor::getSensorData()
{
    getTemperature();
    getHumidity();
    getAirQuality();
    getIrValue();

    sensorData.updated = true;
    return sensorData;
}
