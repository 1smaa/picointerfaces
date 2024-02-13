#ifndef ESP01S_H
#define ESP01S_H

#include "hardware/uart.h"
#include <string>
#include <vector>
#include <stdexcept>
#include "pico/stdlib.h"

#define uint unsigned int

struct WiFi{
    std::string bssid;
    uint security;
    int signal;
    std::string pwd;
    WiFi(std::string response){
        bool rssi,bssid,chNum;
        size_t start=response.find("CWLAP:(");
        if(start==std::string::npos){
            return;
        }
        start+=6;
        int count=0;
        printf("%s\n",response.c_str());
        bool finished=false;
        while(!finished){
            size_t end=response.find(",",start+1);
            if(end==std::string::npos){
                finished=true;
            }
            switch(count){
                case 1:
                    this->bssid=response.substr(start+1,end-start-2);
                    break;
                case 2:
                    this->signal=std::stoi(response.substr(start,end-start));
                    break;
                case 4:
                    this->security=std::stoi(response.substr(start,end-start));
                    break;
                default:
                    break;
            }
            start=end+1;
            count++;
        }
    };
    WiFi() {};
};

namespace ESP01S{
    class ESP01S{
    protected:
        uart_inst_t* uartID;
        int tx,rx;
        int rate;
        std::string wifi, pwd;
    public:
        ESP01S(uart_inst_t* id,int tx,int rx): uartID(id), tx(tx), rx(rx){};
        std::string command(const std::string& command);
        void set_wifi(WiFi wifi);
        WiFi query_wifi(int maxAttempts);
        void setup();
        void reset(int pin);
        bool send(std::string s);
        std::string receive();
        ~ESP01S();
    };
    class SoftAP : public ESP01S{
    public:
        SoftAP(uart_inst_t* id,int tx,int rx) : ESP01S(id,tx,rx){};
        bool set_softap();
        std::string configure_softap(std::string ssid,std::string pwd,int channel,int encryption);
        bool start_tcp(int port);
        bool send(std::string s);
    };
    class Station : public ESP01S{
    public:
        Station(uart_inst_t* id,int tx,int rx) : ESP01S(id,tx,rx){};
        bool set_station();
        bool establish_tcp(int port);
    };
}

#endif