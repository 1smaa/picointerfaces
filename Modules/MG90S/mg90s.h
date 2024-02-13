#ifndef MG90S_H
#define MG90S_H

#include "hardware/pwm.h"
#include "pico/stdlib.h"

class MG90S{
protected:
    int pin;
    int angle;
    pwm_config config;
    void setup();
    int deg_to_level(int deg);
public:
    MG90S();
    MG90S(int pin);
    virtual void move(int angle) = 0;
    bool base();
    ~MG90S();
};

class Continuous: public MG90S{
public:
    Continuous() : MG90S() {};
    Continuous(int pin) : MG90S(pin) {};
    void move(int angle) override;
};

class Discrete: public MG90S{
public:
    Discrete() : MG90S(){};
    Discrete(int pin) : MG90S(pin){};
    void move(int angle) override;
};
    

#endif