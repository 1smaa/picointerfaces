#ifndef HC05_H
#define HC05_H

#include "buffer.h"

class HC05{
private:
    bool paired;
    int snd_pin,rcv_pin;
public:
    HC05();
    HC05(int snd_pin,int rcv_pin,int state_pin);
    template<typename T>
    void send(Buffer::Buffer<T> bits);
    template<typename T>
    Buffer::Buffer<T> receive();
};

#endif