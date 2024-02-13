#ifndef HC05_H
#define HC05_H

#include "buffer.h"
#include "hardware/uart.h"

namespace HC05{
    class HC05{
    protected:
        int txd,rxd,en,state;
        int rate;
        int id;
        uart_inst_t* uart_id;
        virtual std::string setup() = 0;
        void send_command(const char* command);
    public:
        HC05(uart_inst_t* id);
        HC05(uart_inst_t* id, int txd,int rxd,int en);
        HC05(uart_inst_t* id, int txd,int rxd,int en,int baud_rate);
        virtual bool open_connection() = 0;
        bool ready();
        void send(Buffer::Buffer<char> b);
        Buffer::Buffer<char> receive();
        HC05(const HC05&) = delete;
        HC05& operator=(const HC05&) = delete;
    };
    class Master : protected HC05{
        std::string setup() override;
        bool open_connection() override;
    };
    class Slave : protected HC05{
        std::string setup() override;
        bool open_connection() override;
    };
}

#endif