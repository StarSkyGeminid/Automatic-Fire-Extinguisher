#include "big_led.h"

BigLed::BigLed()
{
    mx.begin();
    mx.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
    mx.clear();
    onChange = false;
    status = false;
}

void BigLed::on()
{
    status = true;
    onChange = true;
}

void BigLed::off()
{
    status = false;
    onChange = true;
}

void BigLed::run()
{
    if (status)
    {
        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                mx.setPoint(y, x, true);
            }
        }
    }
    else if (onChange)
    {
        mx.clear();
        onChange = false;
    }
}