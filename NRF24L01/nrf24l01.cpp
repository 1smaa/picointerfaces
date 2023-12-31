#include "nrf24l01.h"
using uint = unsigned int;
using uchar = unsigned char;
#define DEFAULT_RATE 4000000
// registers needed for reading and writing registers
#define R_REGISTER 0x0
#define W_REGISTER 0x20
const uchar R_RX_PAYLOAD = 0x61;
const uchar W_TX_PAYLOAD = 0xA0;
#define FLUSH_TX 0xE0
#define FLUSH_RX 0xE2
#define REUSE_TX_PL 0xE3
#define NOP 0xFF

//register addresses
#define CONFIG 0x00
#define EN_AASHOCK 0x01
#define EN_RXADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS 0x07
#define OBSERVE_TX 0x08
#define CD 0x09
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17

//masks for config register
#define PRIM_RX_MASK 0x01
#define PWR_UP_MAKS 0x02
#define CRCO_MASK 0x04
#define EN_CRC_MASK 0x08
#define MASK_MAX_RT_MASK 0x10
#define MASK_TX_DS_MASK 0x20
#define MASK_RX_DR_MASK 0x40

NRF24L01::NRF24L01::NRF24L01(int tx, int rx, int sck, int ce, int csn, auto spi)
    : tx(tx), rx(rx), sck(sck), ce(ce), csn(csn), spi(spi) {};

void NRF24L01::NRF24L01::write_register(uchar addr,uchar value){
    gpio_put(this->csn,0); // power up
    addr|=W_REGISTER; // create the instruction byte
    spi_write_blocking(this->spi,&addr,1); // write instruction
    spi_write_blocking(this->spi,&value,1); // write register value
    gpio_put(this->csn,1); // power down
}

uchar NRF24L01::NRF24L01::read_register(uchar addr){
    uchar tx_data[2] = {0};
    uchar rx_data[2] = {0};
    tx_data[0] = addr  | 0x00;
    gpio_put(this->csn, 0);
    spi_write_read_blocking(this->spi, tx_data, rx_data, 2);
    gpio_put(this->csn, 1);
    return rx_data[1];
}

void NRF24L01::RX::setup(){
    stdio_init_all();
    spi_init(this->spi,this->rate);
    gpio_set_function(this->sck,GPIO_FUNC_SPI);
    gpio_set_function(this->rx,GPIO_FUNC_SPI);
    gpio_init(this->ce);
    gpio_init(this->csn);
    gpio_set_dir(this->ce,GPIO_OUT);
    gpio_set_dir(this->csn,GPIO_OUT);
    gpio_put(this->ce,0);
    gpio_put(this->csn,1);
    this->write_register(RF_CH,76); // set frequency to 76
    this->write_register(RF_SETUP,0x07); // set data rate to 1mbps
    this->write_register(EN_RXADDR, 0x01); // activate pipe 0
    this->write_register(CONFIG, PWR_UP_MAKS | PRIM_RX_MASK); // config register, power up and rx mode
    sleep_ms(10); // delay to stabilize
    this->write_register(FLUSH_RX,0); // flush the rx fifo
}

bool NRF24L01::RX::ready() {
    uchar status = read_register(STATUS);
    return (status & (1 << 6)) != 0;
}

int NRF24L01::RX::get_length(uchar addr){
    return this->read_register(addr);
}

uchar NRF24L01::RX::read(uchar* data, int length) {
    gpio_put(this->csn, 0);  // Select the NRF24L01 module
    spi_write_blocking(this->spi, &R_RX_PAYLOAD, 1);  // Send the Read Payload command
    // Read the payload data
    spi_read_blocking(this->spi, NOP, data, length);
    gpio_put(this->csn, 1);  // Deselect the NRF24L01 module
    // Clear the RX_DR (data ready) flag in the status register
    this->write_register(STATUS, (1 << 6));
    return length;  // Return the length of the received payload
}

void NRF24L01::TX::setup(){
    stdio_init_all();
    spi_init(this->spi,this->rate); // initiate spi
    gpio_set_function(this->sck,GPIO_FUNC_SPI); // set sck function
    gpio_set_function(this->tx,GPIO_FUNC_SPI); // set tx pin
    gpio_init(this->ce); // initialize ce and csn pin and set gpio direction to out
    gpio_init(this->csn);
    gpio_set_dir(this->ce,GPIO_OUT);
    gpio_set_dir(this->csn,GPIO_OUT);
    gpio_put(this->ce,0);
    gpio_put(this->csn,1);
    this->write_register(RF_CH,76); // set module frequency
    this->write_register(RF_SETUP,0x07); // set data rate 1 mbps
    this->write_register(CONFIG,this->read_register(CONFIG) & !PRIM_RX_MASK); // set tx mode
    sleep_ms(10); // delay to stabilize
    this->write_register(FLUSH_TX,0x0); // flush the tx fifo
}

void NRF24L01::TX::send(const uchar* buffer){
    gpio_put(this->csn,0); // select the module
    spi_write_blocking(this->spi,&W_TX_PAYLOAD,1); // write tx payload instruction
    spi_write_blocking(this->spi,buffer,32); // write payload with 32 bytes length
    gpio_put(this->ce,1); // pulse ce pin to send
    sleep_ms(10);
    gpio_put(this->ce,0);
    gpio_put(this->csn,1); // deselect the module
}

void NRF24L01::RX::set_pipe_addr(uchar pipe_addr,uint addr){
    this->write_register(pipe_addr,addr);
}

uint NRF24L01::RX::get_addr(uchar pipe_addr){
    return this->read_register(pipe_addr);
}

void NRF24L01::TX::set_tx_addr(uint addr){
    this->write_register(TX_ADDR,addr);
}