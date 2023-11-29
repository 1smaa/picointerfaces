#include <stdio.h>
#include "pico/stdlib.h"
#include "l9110s.h"

L9110S::startup(){
    gpio_init(this->pin1,this->pin2);
    gpio_set_dir(this->pin1,GPIO_OUT);
    gpio_set_dir(this->pin2,GPIO_OUT);
    stdio_init_all();
}

L9110S::L9110S(){
    this->pin1=0;
    this->pin2=1;
    this->startup();
    gpio_put(this->pin1,0);
    gpio_put(this->pin2,0);
    this->state=0;
}

L9110S:L9110S(int pin1,int pin2){
    this->pin1=pin1;
    this->pin2=pin2;
    this->startup();
    gpio_put(this->pin1,0);
    gpio_put(this->pin2,0);
    this->state=0;
}

L9110S::L9110S(int pin1,int pin2,int state){
    this->pin=pin1;
    this->pin2=pin2;
    this->startup();
    this->state=state;
    gpio_put(this->pin1,state == 1);
    gpio_put(this->pin2,state == -1);
}

void L9110S::fw(){
    this->state=1;
    gpio_put(this->pin1,1);
    gpio_put(this->pin2,0);
}

void L9110S::bw(){
    this->state=-1;
    gpio_put(this->pin1,0);
    gpio_put(this->pin2,1);
}

void L9110S::stop(){
    this->state=0;
    gpio_put(this->pin1,0);
    gpio_put(this->pin2,0);
}