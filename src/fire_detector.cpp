#include "fire_detector.h"

FireDetector::FireDetector(SensorData *sensorData) : sensorData(sensorData)
{
    temperature[0] = -127;
    temperature[1] = -127;
    temperature[2] = -127;
    temperature[3] = -127;
    temperature[4] = -127;

    airQuality[0] = -127;
    airQuality[1] = -127;
    airQuality[2] = -127;
    airQuality[3] = -127;
    airQuality[4] = -127;
}

void FireDetector::begin()
{
    for (int i = 0; i < 5; i++)
    {
        temperature[i] = 0;
        airQuality[i] = 0;
    }
}

bool FireDetector::fireDetected()
{
    return byIrValue() || byTemperature();
}

bool FireDetector::gasLeakDetected()
{
    return byAirQuality();
}

bool FireDetector::byTemperature()
{
    float temp = sensorData->temperature;

    if (temp == NAN)
    {
        return false;
    }

    int tempDiff = 0;
    uint8_t totalData = 0;

    for (int i = 0; i < 4; i++)
    {
        if (temperature[i] != -127)
        {
            tempDiff += temperature[i] - temperature[i + 1];
            temperature[i] = temperature[i + 1];
            totalData++;
        }
    }

    temperature[totalData] = temp;

    return tempDiff / totalData >= 2 || temp >= 50;
}

bool FireDetector::byAirQuality()
{
    float quality = sensorData->airQuality;

    Serial.println(quality);

    int qualityDiff = 0;
    uint8_t totalData = 0;

    for (int i = 0; i < 4; i++)
    {
        if (airQuality[i] != -127)
        {
            qualityDiff += airQuality[i] - airQuality[i + 1];
            airQuality[i] = airQuality[i + 1];
            totalData++;
        }
    }

    airQuality[totalData] = quality;

    return qualityDiff / totalData >= 20 || quality > 300;
}

bool FireDetector::byIrValue()
{
    return sensorData->irValue > 500;
}