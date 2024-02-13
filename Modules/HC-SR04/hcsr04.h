#ifndef HCSR04_H
#define HCSR04_H

#include "pico/stdlib.h"

class HCSR04{
private:
    int trig_pin,echo_pin;
    float timeout;
    void trigger();
    bool sample();
    void startup();
    int time_us();
public:
    HCSR04();
    HCSR04(int trig_pin,int echo_pin,float timeout);
    float distance_cm();
    float distance_mm();
    ~HCSR04();
};

#endif