#include "ov7670.h"

#define HIGH 1
#define LOW 0

#define DEFAULT_HEIGHT 176
#define DEFAULT_WIDTH 144

OV7670::OV7670(uint dPins_start,uint dPins_end,uint vSync,uint pwrDown,uint href,uint pclk){
    if(dPins_end-dPins_start!=7){
        throw PinError(dPins_end); 
    }
    while(dPins_end-dPins_start){
        this->dPins[dPins_end-dPins_start]=dPins_end;
        gpio_init(dPins_end);
        gpio_set_dir(dPins_end,GPIO_IN);
    }
    this->vSync=vSync;
    gpio_init(vSync);
    gpio_set_dir(vSync,GPIO_IN);
    gpio_pull_up(vSync);
    this->hRef=href;
    gpio_init(href);
    gpio_set_dir(href,GPIO_IN);
    gpio_pull_up(href);
    this->pwrDown=pwrDown;
    gpio_init(pwrDown);
    gpio_set_dir(pwrDown,GPIO_OUT);
    gpio_pull_down(pwrDown);
    gpio_put(pwrDown,LOW);
    this->state=LOW;
    this->pclk=pclk;
    gpio_init(pclk);
    gpio_set_dir(pclk,GPIO_IN);
    gpio_pull_up(pclk);
};

void OV7670::powerUp(){
    gpio_put(pwrDown,HIGH);
    this->state=HIGH;
};

void OV7670::powerDown(){
    gpio_put(pwrDown,LOW);
    this->state=LOW;
};

std::vector<Frame>& OV7670::capture(uint batch_size=30){
    // if module not powere, power-up
    if(!this->state){
        this->powerUp();
    }
    std::vector<Frame> frames(batch_size,Frame(DEFAULT_HEIGHT,DEFAULT_WIDTH));
    while(batch_size--){
        while(gpio_get(this->vSync)){
            sleep_us(10);
        }
        for(int i=0;i<DEFAULT_HEIGHT;i++){
            char* row=new char[DEFAULT_WIDTH];
            int index=0;
            // detect rising edge
            while(gpio_get(this->hRef)){
                sleep_us(10);
            }
            // while falling edge not detected
            while(!gpio_get(this->hRef)&&index<DEFAULT_WIDTH){
                while(gpio_get(this->pclk)){}
                char c;
                for(int j=0;j<8;j++){
                    c|=(gpio_get(this->dPins[j]) & (0x1 << j));
                }
                row[index++]=c;
            }
            frames[batch_size-1].add_row(row);
        }
    }
};
