#ifndef __FIRE_DETECTOR_H__
#define __FIRE_DETECTOR_H__

#include <DHT.h>

#include "sensors/sensor.h"

// struct SensorData
// {
//     bool updated;
//     float temperature;
//     float humidity;
//     float airQuality;
// };

class FireDetector
{
private:
    SensorData *sensorData;

    float temperature[5];
    float airQuality[5];

    bool byTemperature();
    bool byAirQuality();
    bool byIrValue();

public:
    FireDetector(SensorData *sensorData);
    void begin();
    bool fireDetected();
    bool gasLeakDetected();
};

#endif