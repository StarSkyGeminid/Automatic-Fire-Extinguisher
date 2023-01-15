#ifndef __TASK_CORE_H__
#define __TASK_CORE_H__

#define BLYNK_PRINT Serial
#define BUTTON_PIN 33
#define RELAY_PIN 27
#define BUZZER_PIN 15

#define SENSOR_UPDATE_DELAY 3000

#include <esp_task_wdt.h>
#include "config.h"

#include "sensors/sensor.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include "output/water_pump.h"
#include "output/pump_config.h"
#include "output/big_led.h"

#include "fire_detector.h"

SemaphoreHandle_t baton = NULL;

Sensor *sensor;

WaterPump *waterPump;
BigLed *bigLed;
FireDetector *fireDetector;

SensorData sensorData;

bool alertMode = false;
bool gasLeakDetected = false;

unsigned long lastFireDetected = 0;

void onEndTask()
{
    delete waterPump;
    waterPump = NULL;

    delete sensor;
    sensor = NULL;

    delete bigLed;
    bigLed = NULL;

    delete fireDetector;
    fireDetector = NULL;
}

void disableAlertMode()
{
    xSemaphoreTake(baton, portMAX_DELAY);

    alertMode = false;
    bigLed->off();
    waterPump->off();

    digitalWrite(BUZZER_PIN, LOW);

    xSemaphoreGive(baton);
}

void enableAlertMode(bool spray = true)
{
    xSemaphoreTake(baton, portMAX_DELAY);

    alertMode = true;
    bigLed->on();

    if (spray)
    {
        waterPump->on();
    }

    digitalWrite(BUZZER_PIN, HIGH);

    xSemaphoreGive(baton);
}

void updateSensorValue()
{
    xSemaphoreTake(baton, portMAX_DELAY);
    sensorData = sensor->getSensorData();
    xSemaphoreGive(baton);
}

void detectFire()
{
    xSemaphoreTake(baton, portMAX_DELAY);
    bool fireDetected = fireDetector->fireDetected();
    xSemaphoreGive(baton);

    if (fireDetected)
    {
        if (!alertMode)
        {
            enableAlertMode();
        }
        lastFireDetected = millis();
    }
    else if (millis() - lastFireDetected > 30000 && !fireDetected && alertMode)
    {
        disableAlertMode();
    }
}

void detectGas()
{
    xSemaphoreTake(baton, portMAX_DELAY);
    gasLeakDetected = fireDetector->gasLeakDetected();
    xSemaphoreGive(baton);

    if (gasLeakDetected)
    {
        enableAlertMode(false);
    }
    else
    {
        disableAlertMode();
    }
}

void taskCore1(void *param)
{
    bool sendedAlertMode = false;

    uint8_t totalSend = 0;
    unsigned long lastTime = 0;

    for (;;)
    {
        Blynk.run();

        if (millis() - lastTime > SENSOR_UPDATE_DELAY * 2 && sensorData.updated)
        {
            xSemaphoreTake(baton, portMAX_DELAY);

            Blynk.virtualWrite(V0, sensorData.temperature);
            Blynk.virtualWrite(V1, sensorData.humidity);
            Blynk.virtualWrite(V2, sensorData.airQuality);
            sensorData.updated = false;

            xSemaphoreGive(baton);
        }

        if (gasLeakDetected && !sendedAlertMode)
        {
            totalSend++;
            Blynk.notify(F("Warning!\n Gas leak detected!"));
        }
        else if (!gasLeakDetected && sendedAlertMode)
        {
            sendedAlertMode = false;
        }
        else if (gasLeakDetected && totalSend >= 3)
        {
            totalSend = 0;
            sendedAlertMode = true;
        }

        if (alertMode && !sendedAlertMode)
        {
            totalSend++;
            Blynk.notify(F("Warning!\n Fire detected!"));
        }
        else if (!alertMode && sendedAlertMode)
        {
            sendedAlertMode = false;
        }
        else if (alertMode && totalSend >= 3)
        {
            totalSend = 0;
            sendedAlertMode = true;
        }
        delay(10);
    }
}

void taskCore2(void *param)
{
    unsigned long lastTime = 0;

    for (;;)
    {
        if (millis() - lastTime > SENSOR_UPDATE_DELAY)
        {
            updateSensorValue();

            detectFire();
            detectGas();

            lastTime = millis();
        }

        bigLed->run();
    }

    onEndTask();
}

void programInit()
{
    Serial.begin(115200);
    baton = xSemaphoreCreateMutex();

    sensor = new Sensor();
    sensor->init();

    waterPump = new WaterPump(PUMP_PIN);
    waterPump->begin();

    bigLed = new BigLed();
    bigLed->off();

    fireDetector = new FireDetector(&sensorData);
    fireDetector->begin();

    pinMode(BUZZER_PIN, OUTPUT);

    Blynk.begin(BLYNK_AUTH_KEY, WIFI_SSID, WIFI_PASSWORD, SERVER_ADDRESS, SERVER_PORT);
}

#endif