#ifndef MG90S_H
#define MG90S_H

#include "hardware/pwm.h"
#include "pico/stdlib.h"

class MG90S{
private:
    const int BASE_MILLIS=400;
    int pin;
    int angle;
    int base_angle;
    pwm_config config;
    void setup();
    float deg_to_time(int deg);
public:
    MG90S();
    MG90S(int pin);
    MG90S(int pin,int base_angle);
    void move(int angle);
    bool base();
};

#endif