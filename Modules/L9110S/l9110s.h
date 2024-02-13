#ifndef L9110S_H
#define L9110S_H

#include <pair>
#include "PWM/pwm.h"
#include <cstdint>


namespace L9110S{
    class STD{
    private:
        int state;
        int pin1,pin2;
        void startup();
    public:
        STD(int pin1,int pin2);
        STD(int pin1,int pin2,int state);
        void fw();
        void bw();
        void stop();
        std::pair<int,int> get_pins() const;
        ~STD();
    };
    class ADV{
    private:
        PWM::PWM pwm1_,pwm2_;
        void startup();
    public:
        ADV(PWM::PWM pwm1,PWM::PWM pwm2) : pwm1_(pwm1),pwm2_(pwm2){};
        void set(int level);
        void stop();
        ~ADV();
    };
    extern ADV toADV(STD& mod);
}


#endif