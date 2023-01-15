#ifndef __BIG_LED_H__
#define __BIG_LED_H__

#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

#include "big_led_config.h"

class BigLed{
private:
    const int maxX = MAX_DEVICES * 8 - 1;
    const int maxY = 7;

    MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
    bool status;
    bool onChange;

public:
    BigLed();
    void on();
    void off();
    void run();
};

#endif