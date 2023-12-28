#ifndef NRF24L01_H
#define NRF24L01_H
#define uint unsigned int
#define uchar unsigned char

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
        int spi;
        // selected data rate
        int rate;
        // function to write to a register
        void write_register(uchar addr,uchar value);
        // function to read a register
        uchar read_register(uchar addr);
    public:
        NRF24L01(int tx,int rx,int sck,int ce,int csn,int spi);
        NRF24L01(int tx,int rx,int sck,int ce,int spi,int csn);
        // override specifications
        NRF24L01(const NRF24L01&) = delete;
        NRF24L01& operator=(const NRF24L01&) = delete;
        virtual void setup() = 0;
    };
    class RX : protected NRF24L01{
        // setup the module for RX use
        void setup() override;
        // get ready-state of the selected data pipe
        bool ready();
        // read the register
        uchar* read();
        // obtains the length of the data in pipe
        int get_length(int pipe_id);
        void set_pipe_addr(uchar pipe_addr,uint addr);
        uint get_addr(uint pipe_id);
    };
    class TX : protected NRF24L01{
        // setup the module for TX use
        void setup() override;
        // send the buffer through the selected data pipe
        void send(int pipe_id,const uchar* buffer);
        void set_tx_addr(uint addr);
    };
}

#endif