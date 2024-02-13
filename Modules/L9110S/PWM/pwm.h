#ifndef PWM_H
#define PWM_H

#include <cstdint>
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <utility>
#include <stdexcept>

#define DEFAULT_FREQ 50
#define PICO_CLK 125000000

#define CONFIG_PAIR std::pair<uint8_t,uint32_t>

namespace PWM{
    class PWM_error : std::runtime_error {
    public: 
        uint8_t slice_;
        PWM_error(uint8_t slice) : std::runtime_error("PWM Error."), slice_(slice) {};
        const char* what() const noexcept override;
    };
    uint32_t wrap_from_clk(uint32_t clk,uint8_t clkdvd);
    CONFIG_PAIR find_config(uint32_t clk,uint32_t freq);
    class PWM{
    private:
        uint8_t clkdvd_;
        uint32_t wrap_;
        uint8_t pin_;
        uint8_t slice_;
        uint32_t clk_;
        pwm_config config_;
        uint32_t freq_;
        void init();
    public:
        PWM(uint8_t pin) : pin_(pin), clk_(PICO_CLK), freq_(DEFAULT_FREQ) { this->init(); };
        PWM(uint8_t pin,uint32_t freq) : pin_(pin), clk_(PICO_CLK), freq_(freq) { this->init(); };
        PWM(uint8_t pin,uint32_t freq,uint32_t clk) : pin_(pin), clk_(clk), freq_(freq) { this->init(); };

        uint32_t set(float percentage);
        uint32_t set(uint32_t level);
        uint32_t set_pulse(float pulse_us);

        uint8_t get_clkdvd() const;
        uint32_t get_wrap() const;
        uint8_t get_pin() const;
        ~PWM();
    };      
}

#endif