#ifndef NRF24L01_H
#define NRF24L01_H
using uint = unsigned int;
using uchar = unsigned char;

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

namespace NRF24L01{
    /*
        Virtual class for NRF24L01(+) modules both in TX and RX mode.
    */
    class NRF24L01{
    protected:
        // pins
        int tx,rx,sck,ce,csn;
        // spi id on the pico board
        spi_inst_t* spi;
        // selected data rate
        int rate;
        // function to write to a register
        void write_register(uchar addr,uchar value);
        // function to read a register
        uchar read_register(uchar addr);
    public:
        NRF24L01(int tx,int rx,int sck,int ce,int csn,auto spi);
        // override specifications
        NRF24L01(const NRF24L01&) = delete;
        NRF24L01& operator=(const NRF24L01&) = delete;
        virtual void setup() = 0;
    };
    class RX : protected NRF24L01{
    public:
        RX(int tx,int rx,int sck,int ce,int csn,auto spi) : NRF24L01(tx,rx,sck,ce,csn,spi){};
        // setup the module for RX use
        void setup() override;
        // get ready-state of the selected data pipe
        bool ready();
        // read the register
        uchar read(uchar* data, int length);
        // obtains the length of the data in pipe
        int get_length(uchar addr);
        void set_pipe_addr(uchar pipe_addr,uint addr);
        uint get_addr(uchar pipe_addr);
    };
    class TX : protected NRF24L01{
    public:
        TX(int tx,int rx,int sck,int ce,int csn,auto spi) : NRF24L01(tx,rx,sck,ce,csn,spi){};
        // setup the module for TX use
        void setup() override;
        // send the buffer through the selected data pipe
        void send(const uchar* buffer);
        void set_tx_addr(uint addr);
    };
}

#endif