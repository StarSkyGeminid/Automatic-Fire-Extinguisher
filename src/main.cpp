
#include <Arduino.h>

#include "core_task.h"

TaskHandle_t Task1;
TaskHandle_t Task2;

// #include "DHT.h"

// #define DHT_SENSOR_PIN (4)
// #define DHT_TYPE (DHT11)

// DHT *dht;


void setup()
{
  programInit();
  // Serial.begin(115200);
  // dht = new DHT(DHT_SENSOR_PIN, DHT_TYPE);

  // dht->begin();

  xTaskCreatePinnedToCore(
      taskCore1, /* Function to implement the task */
      "Task1",   /* Name of the task */
      10000,     /* Stack size in words */
      NULL,      /* Task input parameter */
      0,         /* Priority of the task */
      &Task1,    /* Task handle. */
      0);        /* Core where the task should run */

  xTaskCreatePinnedToCore(
      taskCore2, /* Function to implement the task */
      "Task2",   /* Name of the task */
      10000,     /* Stack size in words */
      NULL,      /* Task input parameter */
      1,         /* Priority of the task */
      &Task2,    /* Task handle. */
      1);        /* Core where the task should run */

}

void loop()
{
  // Serial.println(dht->readTemperature());
  delay(100);
}