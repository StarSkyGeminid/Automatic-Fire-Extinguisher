#ifndef __PUMP_H__
#define __PUMP_H__

class WaterPump
{
private:
    int pin;
    bool state;

public:
    WaterPump(int pumpPin) : pin(pumpPin){}
    
    void begin();
    void on();
    void off();
    bool getState();
};

#endif