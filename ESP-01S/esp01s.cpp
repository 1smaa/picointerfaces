#include "esp01s.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <string>
#define DEFAULT_BAUD_RATE 115200
#define HIGH 1
#define LOW 0
#define DEFAULT_PASSWORD esp01sPico

#include <chrono>

std::string ESP01S::ESP01S::command(const std::string& command, char termination) {
    uart_puts(this->uartID, command.c_str());
    std::string output;
    do{ 
        output = "";
        while (uart_is_readable(this->uartID)) {
            output += uart_getc(this->uartID);
        }
    }
    while(output.find("busy p...")!=std::string::npos||(termination!='\0'&&output.find(termination)==std::string::npos));
    printf("Input %s - Ouput %s\n",command.c_str(),output.c_str());
    return output;
}


void ESP01S::ESP01S::setup(){
    uart_init(this->uartID, DEFAULT_BAUD_RATE);
    gpio_set_function(this->tx, GPIO_FUNC_UART);
    gpio_set_function(this->rx, GPIO_FUNC_UART);
    gpio_pull_up(this->rx);
    uart_set_translate_crlf(this->uartID, false);
    uart_set_hw_flow(this->uartID, false, false);
    uart_set_format(this->uartID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(this->uartID, true);
    this->command("AT+RST\r\n",'\0');
    this->command("AT+UART_DEF=115200,8,1,0,0\r\n",'\0');
    this->command("ATE0\r\n",'\0');
}

void ESP01S::ESP01S::reset(int pin){
    gpio_init(pin);
    gpio_set_dir(pin,GPIO_OUT);
    gpio_put(pin,LOW);
    sleep_ms(50);
    gpio_put(pin,HIGH);
    gpio_deinit(pin);
}

void ESP01S::ESP01S::set_wifi(WiFi wifi){
    std::string command="AT+CWJAP=\""+wifi.bssid+"\",\""+wifi.pwd+"\"\r\n";
    printf(this->command(command,'\0').c_str());
};

WiFi ESP01S::ESP01S::query_wifi(int maxAttempts){
    std::string response="";
    response=this->command("AT+CWLAP\r\n",')');
    return WiFi(response);
};

bool ESP01S::SoftAP::set_softap(){
    return this->command("AT+CWMODE=2\r\n",'\0').find("ERROR")==std::string::npos;
};

std::string ESP01S::SoftAP::configure_softap(std::string ssid,std::string password,int channel,int security){
    return this->command("AT+CWSAP=\""+ssid+"\",\""+password+"\","+std::to_string(channel)+","+std::to_string(security)+"\r\n",'\0');
};

bool ESP01S::SoftAP::start_tcp(int port){
    this->command("AT+CIPMUX=1\r\n",'\0');
    return this->command("AT+CIPSERVER=1,80\r\n",'\0').find("ERROR")==std::string::npos;
};

bool ESP01S::Station::set_station(){
    return this->command("AT+CWMODE=1\r\n",'\0').find("ERROR")==std::string::npos;
};

bool ESP01S::Station::establish_tcp(int port){
    std::string r=this->command("AT+CIFSR\r\n",'\0');
    size_t pos=r.find("SoftAPIP");
    if(pos==std::string::npos) return false;
    pos+=10;
    size_t end=r.find("\"",pos);
    std::string ip=r.substr(pos,end-pos);
    return this->command("AT+CIPSTART=0,\"TCP\",\""+ip+"\","+std::to_string(port)+"\r\n",'\0').find("ERROR")==std::string::npos;
};

bool ESP01S::ESP01S::send(std::string s){
    this->command("AT+CIPSEND="+std::to_string(s.length())+"\r\n",'\0');
    sleep_ms(50);
    return this->command(s+"\r\n",'\0').find("SEND OK")!=std::string::npos;
};

std::string ESP01S::ESP01S::receive(uint size){
    return this->command("AT+CIPRECV="+std::to_string(size)+"\r\n",'\0');
};
