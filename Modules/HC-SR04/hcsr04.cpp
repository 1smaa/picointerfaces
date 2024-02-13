#include <stdio.h>
#include "hcsr04.h"
#include "hardware/gpio.h"

void HCSR04::startup(){
    gpio_init(this->trig_pin);
    gpio_init(this->echo_pin);
    gpio_set_dir(this->trig_pin,GPIO_OUT);
    //gpio_disable_pulls(this->echo_pin);
    gpio_set_dir(this->echo_pin,GPIO_IN);
    stdio_init_all();
}

void HCSR04::trigger(){
    gpio_put(this->trig_pin,0);
    gpio_put(this->trig_pin,1);
    sleep_us(10);
    gpio_put(this->trig_pin,0);
}

bool HCSR04::sample(){
    return gpio_get(this->echo_pin);
}

int HCSR04::time_us(){
    int t=0;
    this->trigger();
    int width=0;
    while(gpio_get(this->echo_pin)==0) tight_loop_contents();
    while(gpio_get(this->echo_pin)==1){
        width++;
        sleep_us(1);
        if(width>timeout) return 0;
    }
    return width;
}

HCSR04::HCSR04(){
    this->trig_pin=0;
    this->echo_pin=1;
    this->timeout=1000000;
    this->startup();
    gpio_put(this->trig_pin,0);
}

HCSR04::HCSR04(int trig_pin,int echo_pin,float timeout){
    this->trig_pin=trig_pin;
    this->echo_pin=echo_pin;
    this->timeout=timeout;
    this->startup();
    gpio_put(this->trig_pin,0);
}

float HCSR04::distance_cm(){
    int t=this->time_us();
    return (t / 2.0f) / 29.1f;
}

float HCSR04::distance_mm(){
    return this->distance_cm()*10;
}

HCSR04::~HCSR04(){
    gpio_deinit(this->echo_pin);
    gpio_deinit(this->trig_pin);
}