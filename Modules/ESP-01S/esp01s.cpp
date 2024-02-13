#include "esp01s.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <string>
#include <cstring>
#define DEFAULT_BAUD_RATE 115200
#define HIGH 1
#define LOW 0
#define DEFAULT_PASSWORD esp01sPico


std::string ESP01S::ESP01S::command(const std::string& command) {
    // Send command
    std::string output="";
    for(int i=0;i<command.length();i++){
        uart_putc(this->uartID,command[i]);
        sleep_us(50);
    }
    sleep_ms(100);
    while(uart_is_readable(this->uartID)){
        output+=uart_getc(this->uartID);
    }
    printf("UART: %d,Input: %s\n Output: %s\n",uart_get_index(this->uartID),command.c_str(),output.c_str());
    return output;
}


void ESP01S::ESP01S::setup(){
    uart_init(this->uartID, DEFAULT_BAUD_RATE);
    gpio_set_function(this->tx, GPIO_FUNC_UART);
    gpio_set_function(this->rx, GPIO_FUNC_UART);
    uart_set_translate_crlf(this->uartID, false);
    uart_tx_wait_blocking(this->uartID);
    uart_set_hw_flow(this->uartID, false, false);
    uart_tx_wait_blocking(this->uartID);
    uart_set_format(this->uartID, 8, 1, UART_PARITY_NONE);
    uart_tx_wait_blocking(this->uartID);
    uart_set_fifo_enabled(this->uartID, true);
    this->command("AT+RST\r\n");
    sleep_ms(100);
    this->command("AT+UART_CUR=115200,8,1,0,0\r\n");
    this->command("ATE1\r\n");
    //this->command("AT+CWQAP\r\n");
    //this->command("AT+RFPOWER=82\r\n");
}

void ESP01S::ESP01S::reset(int pin){
    gpio_init(pin);
    gpio_set_dir(pin,GPIO_OUT);
    gpio_put(pin,LOW);
    sleep_ms(50);
    gpio_put(pin,HIGH);
    printf("Resetted. \n");
    gpio_deinit(pin);
}

void ESP01S::ESP01S::set_wifi(WiFi wifi){
    std::string command="AT+CWJAP_CUR=\""+wifi.bssid+"\",\""+wifi.pwd+"\"\r\n";
    printf(this->command(command).c_str());
};

WiFi ESP01S::ESP01S::query_wifi(int maxAttempts){
    std::string command="AT+CWLAP\r\n";
    for(int i=0;i<command.length();i++){
        uart_putc(this->uartID,command[i]);
        sleep_us(50);
    }
    std::string output="";
    while(output.find(")")==std::string::npos&&maxAttempts--){
        while(uart_is_readable(this->uartID)){
            output+=uart_getc(this->uartID);
        }
        sleep_ms(100);
    }
    printf("Input: AT+CWLAP \n Output: %s",output.c_str());
    return WiFi(output);
};

ESP01S::ESP01S::~ESP01S(){
    uart_deinit(this->uartID);
    gpio_deinit(this->tx);
    gpio_deinit(this->rx);
}

bool ESP01S::SoftAP::set_softap(){
    return this->command("AT+CWMODE_CUR=2\r\n").find("ERROR")==std::string::npos;
};

std::string ESP01S::SoftAP::configure_softap(std::string ssid,std::string password,int security){
    this->command("AT+CIPAP_CUR=\"192.168.1.100\"\r\n");
    return this->command("AT+CWSAP=\""+ssid+"\",\""+password+"\",0,"+std::to_string(security)+"\r\n");
};

bool ESP01S::SoftAP::start_tcp(int port){
    this->command("AT+CIPMUX=1\r\n");
    return this->command("AT+CIPSERVER=1,"+std::to_string(port)+"\r\n").find("ERROR")==std::string::npos;
};

bool ESP01S::Station::set_station(){
    this->command("AT+CIPMUX=0\r\n");
    return this->command("AT+CWMODE_CUR=1\r\n").find("ERROR")==std::string::npos;
};

bool ESP01S::Station::establish_tcp(int port){
    return this->command("AT+CIPSTART=\"TCP\",\"192.168.1.100\","+std::to_string(port)+"\r\n").find("ERROR")==std::string::npos;
};

bool ESP01S::ESP01S::send(std::string s){
    this->command("AT+CIPSENDEX="+std::to_string(s.length())+"\r\n");
    sleep_ms(500);
    return this->command(s).find("SEND OK")!=std::string::npos;
};

bool ESP01S::SoftAP::send(std::string s){
    this->command("AT+CIPSENDEX=0,"+std::to_string(s.length()+2)+"\r\n");
    sleep_ms(1000);
    return this->command(s+"\0").find("SEND OK")!=std::string::npos;
}

std::string ESP01S::ESP01S::receive(int length){
    return this->command("+IPD,"+std::to_string(length)+"\r\n");
};

std::string ESP01S::SoftAP::receive(int length){
    return this->command("+IPD,0,"+std::to_string(length)+"\r\n");
}
