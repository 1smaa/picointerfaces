#ifndef L9110S_H
#define L9110S_H

class L9110S{
private:
    int state;
    std::pair<int,int> m1,m2;
    void startup();
public:
    L9110S();
    L9110S(int pin1,int pin2);
    L9110S(int pin1,int pin2,int state);
    void fw();
    void bw();
    void stop();
};

#endif