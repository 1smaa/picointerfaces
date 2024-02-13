#ifndef OV7670_H
#define OV7670_H

#include <stdexcept>
#include <string>
#include <vector>
#include "pico/stdlib.h"
#include "frame.h"

#define uint unsigned int

class PinError : std::runtime_error{
public:
    int pin;
    PinError(int pin) : pin(pin), std::runtime_error("Pin Error"){};
};

class OV7670{
private:
    uint dPins[8];
    uint vSync;
    uint pwrDown;
    uint hRef;
    uint pclk;
    bool state;
public:
    OV7670(uint dPins_start,uint dPins_end,uint vSync,uint pwrDown,uint href, uint pclk);
    void powerUp();
    void powerDown();
    std::vector<Frame>& capture(uint batch_size=30);
    ~OV7670();
};

#endif