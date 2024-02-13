#include "mg90s.h"
#include <stdio.h>
#include <string>

#define LEFT 1
#define RIGHT 2
#define STOP 1.5

#define W 540.0f

void MG90S::setup() {
    gpio_set_function(this->pin, GPIO_FUNC_PWM);
    unsigned int slice = pwm_gpio_to_slice_num(this->pin);
    this->config = pwm_get_default_config();
    // Set the clock divider to achieve a PWM frequency of 50 Hz
    pwm_config_set_clkdiv_int(&(this->config), 100); // 125 MHz / 50 Hz / 25000 wrap value
    pwm_config_set_wrap(&(this->config), 25000); // Wrap value for 50 Hz PWM frequency
    pwm_init(slice, &(this->config), true);
    stdio_init_all();
    pwm_set_gpio_level(this->pin, 1875); // Set initial PWM level (neutral position)
}

int MG90S::deg_to_level(int deg) {
    // Map the angle to a pulse width between 1 ms and 2 ms
    return ((deg / 180.0f) + 1.0f) * 25000 / 20; // 1 ms + (angle / 90) * 1 ms
}

MG90S::MG90S() {
    this->pin = 0;
    this->angle = 90;
    this->setup();
}

MG90S::MG90S(int pin) {
    this->pin = pin;
    this->angle = 90;
    this->setup();
}

void Discrete::move(int angle) {
    // Convert the angle to a pulse width and set the PWM level
    pwm_set_gpio_level(this->pin, this->deg_to_level(angle)); // Convert microseconds to nanoseconds
    this->angle = angle;
}

void Continuous::move(int angle){
    if(angle < this->angle){
        pwm_set_gpio_level(this->pin, (LEFT*25000)/20);
        sleep_ms(((this->angle-angle)/W)*1000);
        pwm_set_gpio_level(this->pin, (STOP*25000)/20);
    } else if(angle > this->angle){
        pwm_set_gpio_level(this->pin, (RIGHT*25000)/20);
        sleep_ms(((angle-this->angle)/W)*1000);
        pwm_set_gpio_level(this->pin, (STOP*25000)/20);
    }
    printf(std::to_string(this->angle-angle).c_str());
    printf("\n");
    this->angle=angle;
}

bool MG90S::base() {
    return this->angle == 0;
}

MG90S::~MG90S(){
    gpio_deinit(this->pin);
}
