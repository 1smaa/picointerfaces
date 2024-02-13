#include "hc05.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <string>
#include "hardware/uart.h"
#define DEF_BAUD_RATE 9600

HC05::HC05::HC05(int id) : id(id), uart_id(!id ? uart0 : uart1), en(3), rxd(1), txd(0), state(0), rate(DEF_BAUD_RATE) {}

HC05::HC05::HC05(int id, int txd,int rxd,int en): id(id), uart_id(!id ? uart0 : uart1), en(en), txd(txd), rxd(rxd), state(0), rate(DEF_BAUD_RATE) {}

HC05::HC05::HC05(int id, int txd,int rxd,int en,int baud_rate) : id(id), uart_id(!id ? uart0 : uart1), txd(txd), rxd(rxd), en(en), state(0), rate(baud_rate) {}

void HC05::HC05::send_command(const char* command){
    while(*command){
        uart_putc(this->uart_id,*command++);
    }
}

std::string HC05::Master::setup(){
    uart_init(this->uart_id,this->rate);
    gpio_set_function(this->txd,GPIO_FUNC_UART);
    gpio_set_function(this->rxd,GPIO_FUNC_UART);
    this->send_command("AT");
    this->send_command("AT+ROLE=1\r\n");
    this->send_command("AT+PSWD=4321\r\n");
    this->send_command("AT+CMODE=1\r\n");
    return "";
}

std::string HC05::Slave::setup(){
    uart_init(this->uart_id,this->rate);
    gpio_set_function(this->txd,GPIO_FUNC_UART);
    gpio_set_function(this->rxd,GPIO_FUNC_UART);
    this->send_command("AT+ROLE=0\r\n");
    this->send_command("AT+PSWD=4321\r\n");
    this->send_command("AT+CMODE=1\r\n");
    this->send_command("AT+INQM\r\n");
    this->send_command("AT+ADDR?\r\n");
    std::string address="";
    while(uart_is_readable(this->uart_id)){
        address+=uart_getc(this->uart_id);
    }
    return address;
}

bool HC05::HC05::ready(){
    return uart_is_readable(this->uart_id);
}

void HC05::HC05::send(Buffer::Buffer<char> b){
    uart_puts(this->uart_id,b.getRaw());
}

Buffer::Buffer<char> HC05::HC05::receive(){
    std::string rcv=uart_gets(this->uart_id);
    Buffer::Buffer<char> b((int)sizeof(rcv),(unsigned char)rcv[0]);
    for(int i=1;i<rcv.size();i++){
        b.push_back(rcv[i]);
    }
    return b;
}