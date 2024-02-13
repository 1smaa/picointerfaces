#include "uart.h"

const char* UART::UART_error::what() const{
    std::string message = "Error while using UART"+std::to_string(uart_get_index(this->uart_))+".";
    return message.c_str();
};

const char* UART::UART_bad_pin::what() const {
    std::string message = "Bad Pin: " + std::to_string(this->bad_pin_);
    return message.c_str();
};

uint8_t UART::default_tx(uart_inst_t* uart_id){
    switch(uart_id){
        case uart0:
            return 0;
        case uart1:
            return 4;
        default:
            throw UART_error(uart_id);
    }
}

uint8_t UART::default_rx(uart_inst_t* uart_id){
    switch(uart_id){
        case uart0:
            return 1;
        case uart1:
            return 5;
        default:
            throw UART_error(uart_id);
    }
}

void UART::check_pins(uart_inst_t* uart_id,uint8_t tx,uint8_t rx){
    if(uart_id==uart0&&((tx!=0&&tx!=12)&&tx!=16)){
        throw UART_bad_pin(tx);
    }
    if(uart_id==uart0&&((rx!=1&&rx!=13)&&rx!=17)){
        throw UART_bad_pin(rx);
    }
    if(uart_id==uart1&&(tx!=4&&tx!=8)){
        throw UART_bad_bin(tx);
    }
    if(uart_id==uart1&&(rx!=5&&rx!=9)){
        throw UART_bad_bin(rx);
    }
}

void UART::UART::init(){
    stdio_init_all();
    gpio_init(this->tx_);
    gpio_init(this->rx_);
    uart_init(this->ID_,this->baud_rate_);
    gpio_set_function(this->tx_, GPIO_FUNC_UART);
    gpio_set_function(this->rx_, GPIO_FUNC_UART);
    uart_set_translate_crlf(this->ID_, false);
    uart_tx_wait_blocking(this->ID_);
    uart_set_hw_flow(this->ID_, false, false);
    uart_set_format(this->ID_, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(this->ID_, true);
}

bool UART::UART::ready() const{
    return uart_is_writeable(this->ID_);
}

bool UART::UART::readable() const{
    return uart_is_readable(this->ID_);
}

void UART::UART::send(const char* s){
    while(*s){
        uart_putc(this->ID_,*s++);
        sleep_us(50);
    }
}

const char* UART::UART::read(){
    std::string output="";
    while(uart_is_readable(this->ID_)){
        output+=uart_getc(this->ID_);
    }
    return output.c_str();
}

const char* UART::UART::send_read(const char* s){
    this->send(s);
    sleep_us(1000);
    return this->read();
}

uart_inst_t* UART::UART::get_raw() const {
    return this->ID_;
}

uint8_t UART::UART::get_index() const {
    return uart_get_index(this->ID_);
}

UART::UART::~UART(){
    uart_deinit(this->ID_);
    gpio_deinit(this->tx);
    gpio_deinit(this->rx);
}