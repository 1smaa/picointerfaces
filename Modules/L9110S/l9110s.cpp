#include <stdio.h>
#include "pico/stdlib.h"
#include "l9110s.h"

#define DEFAULT_CYCLE 10000

void L9110S::STD::startup(){
    gpio_init(this->pin1,this->pin2);
    gpio_set_dir(this->pin1,GPIO_OUT);
    gpio_set_dir(this->pin2,GPIO_OUT);
    gpio_put(this->pin1,0);
    gpio_put(this->pin2,0);
    this->state=0;
    stdio_init_all();
}

L9110S::STD::STD(int pin1,int pin2){
    this->pin1=pin1;
    this->pin2=pin2;
    this->startup();
}

L9110S::STD::STD(int pin1,int pin2,int state){
    this->pin1=pin1;
    this->pin2=pin2;
    this->startup();
    this->state=state;
    gpio_put(this->pin1,state == 1);
    gpio_put(this->pin2,state == -1);
}

void L9110S::STD::fw(){
    this->state=1;
    gpio_put(this->pin1,1);
    gpio_put(this->pin2,0);
}

void L9110S::STD::bw(){
    this->state=-1;
    gpio_put(this->pin1,0);
    gpio_put(this->pin2,1);
}

void L9110S::STD::stop(){
    this->state=0;
    gpio_put(this->pin1,0);
    gpio_put(this->pin2,0);
}

std::pair<int,int> L9110S::STD::get_pins() const{
    return std::pair<int,int>(this->pin1,this->pin2);
}

L9110S::STD::~STD(){
    gpio_deinit(this->pin1);
    gpio_deinit(this->pin2);
}

void L9110S::ADV::set(int level){
    if(level>25) level=25;
    if(level<-25) level=-25;
    float p=(level)/100.0f;
    if(p<0){
        this->pwm1_.set(0.0f);
        this->pwm2_.set(p);
    } else {
        this->pwm2_.set(0.0f);
        this->pwm1_.set(p);
    }
}

void L9110S::ADV::stop(){
    this->pwm1_.set(0.0f);
    this->pwm2_.set(0.0f);
}

L9110S::ADV::~ADV(){
    gpio_deinit(this->pwm1_.get_pin());
    gpio_deinit(this->pwm2_.get_pin());
}

L9110S::ADV L9110S::toADV(L9110S::STD& mod){
    std::pair<int,int> pins=mod.get_pins();
    PWM::PWM pwm1(pins.first,DEFAULT_CYCLE);
    PWM::PWM pwm2(pins.second,DEFAULT_CYCLE);
    return ADV(pwm1,pwm2);
}