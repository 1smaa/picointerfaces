#include "mg90s.h"
#include <stdio.h>

void MG90S::setup(){
    gpio_set_function(this->pin,GPIO_FUNC_PWM);
    unsigned int slice=pwm_gpio_to_slice_num(this->pin);
    this->config=pwm_get_default_config();
    pwm_config_set_clkdiv(&(this->config),64.0f);
    pwm_config_set_wrap(&(this->config),39062.0f);
    pwm_init(slice,&(this->config),true);
    stdio_init_all();
    pwm_set_gpio_level(this->pin,(int)((this->BASE_MILLIS/20.0f)*39062.0f));
}

float MG90S::deg_to_time(int deg){
    float temp=((deg/90.0f)+0.5);
    return temp;
}

MG90S::MG90S(){
    this->pin=0;
    this->angle=0;
    this->base_angle=0;
    this->setup();
}

MG90S::MG90S(int pin){
    this->pin=pin;
    this->angle=0;
    this->base_angle=0;
    this->setup();
    this->move(0);
}

MG90S::MG90S(int pin,int base_angle){
    this->pin=pin;
    this->angle=0;
    this->base_angle=base_angle;
    this->setup();
    this->move(0);
}

void MG90S::move(int angle){
    pwm_set_gpio_level(this->pin,(int)((this->deg_to_time(angle+(this->base_angle))/20.0f)*39062.0f));
    this->angle=angle;
}

bool MG90S::base(){
    return this->angle==0;
}