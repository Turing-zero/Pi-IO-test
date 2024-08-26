#ifndef __DISPLAY__
#define __DISPLAY__
#include "lib_io.h"

// 寄存器地址
#define READ_REG 0x00
#define SEND_REG 0x01

#define DEVICE_0 0x48
#define DEVICE_1 0x49
#define DEVICE_2 0x4a
#define DEVICE_3 0x4b

class Display{
    public:
        ~Display();
        void open_screen(int bus=0,int cs=0,int spimode=0,int frequency=1000000,int bitPerWord=8,bool lsbmode=false,int reset=22,int ds=18);
        void write(uint8_t*tx_buf,int length);
        void set_resetio(int value);
        void set_dsio(int value);
        void transfer(uint8_t* tx_buf,uint8_t rx_buf,int length);
        void close();
    private:
        gpio_module *_reset;
        gpio_module *_ds;
        spi_module *_spi;
};
#endif