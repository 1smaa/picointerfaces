#include "pwm.h"

uint32_t PWM::wrap_from_clk(uint32_t clk,uint8_t clkdvd){
    return clk/clkdvd;
}

CONFIG_PAIR PWM::find_config(uint32_t clk,uint32_t freq){
    uint8_t clkdvd=255;
    uint32_t wrap=0;
    while(clkdvd--&&clk%clkdvd){}
    wrap=clk/clkdvd;
    return CONFIG_PAIR(clkdvd,wrap);
}

void PWM::PWM::init(){
    gpio_set_function(this->pin_,GPIO_FUNC_PWM);
    this->slice_=pwm_gpio_to_slice_num(this->pin_);
    this->config_=pwm_get_default_config();
    CONFIG_PAIR p=find_config(this->clk_,this->freq_);
    pwm_config_set_clkdiv_int(&(this->config_), p.first); 
    pwm_config_set_wrap(&(this->config_), p.second); 
    pwm_init(this->slice_, &(this->config_), true);
    stdio_init_all();
}

uint32_t PWM::PWM::set(float percentage){
    uint32_t level=percentage*this->wrap_;
    pwm_set_gpio_level(this->pin_,level);
    return level;
}

uint32_t PWM::PWM::set(uint32_t level){
    pwm_set_gpio_level(this->pin_,level);
    return level;
}

uint32_t PWM::PWM::set_pulse(float pulse_us){
    double pwm_pulse=(1/this->freq_)*1000000;
    int32_t level=(pulse_us*this->wrap_)/pwm_pulse;
    pwm_set_gpio_level(this->pin_,level);
    return level;
}

uint8_t PWM::PWM::get_clkdvd() const{
    return this->clkdvd_;
}
uint32_t PWM::PWM::get_wrap() const{
    return this->wrap_;
}

PWM::PWM::~PWM(){
    gpio_deinit(this->pin_);
}