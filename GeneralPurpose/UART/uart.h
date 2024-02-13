#ifndef UART_H
#define UART_H

#include "pico/stdlib.h"
#include <stdio.h>
#include <cstdint>
#include <stdexcept>
#include "hardware/uart.h"
#include <string>

#define DEFAULT_BAUD_RATE 115200
#define DEFAULT_UART uart0

namespace UART{

    class UART_error : std::runtime_error{
    public:
        uart_inst_t* uart_;
        UART_error() : uart_(uart0) {};
        UART_error(uart_inst_t* uart) : uart_(uart) {};
        const char* what() const override;
    };

    class UART_bad_pin : std::logic_error{
    public:
        uint8_t bad_pin_;
        UART_bad_pin() : bad_pin_(0) {};
        UART_bad_pin(uint8_t pin) : bad_pin_(pin){};
        const char* what() const override;
    };

    uint8_t default_tx(uart_inst_t* uart_id);

    uint8_t default_rx(uart_inst_t* uart_id);

    void check_pins(uart_inst_t* uart_id,uint8_t tx,uint8_t rx);

    class UART{
    private:
        uart_inst_t* ID_;
        uint8_t tx_,rx_;
        uint32_t baud_rate_;
        void init();
    public:
        UART(uint32_t baud_rate = DEFAULT_BAUD_RATE) 
            : ID_(DEFAULT_UART), tx_(default_tx(ID_)), rx_(default_rx(ID_)), baud_rate_(baud_rate){ this->init(); };
        UART(uart_inst_t* uart_id,uint32_t baud_rate = DEFAULT_BAUD_RATE) 
            : ID_(uart_id), tx_(default_tx(ID_)), rx_(default_rx(ID_)){ this->init(); };
        UART(uart_inst_t* uart_id,uint8_t tx,uint8_t rx,uint32_t baud_rate = DEFAULT_BAUD_RATE) 
            : ID_(uart_id), tx_(tx), rx_(rx), baud_rate_(baud_rate) { check_pins(ID_,tx_,rx_); this->init(); };
        void send(const char* s);
        const char* send_read(const char* s);
        const char* read() const;
        
        bool ready() const;
        bool readable() const;

        uart_inst_t* get_raw() const;
        uint8_t get_index() const;
        ~UART();
    };
}

#endif