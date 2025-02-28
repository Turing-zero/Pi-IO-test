#include "lib_io.h"
#include "touchscreen.h"

#define REG  0x02
#define ADDR 0x15

int main(){
    // i2c_module i2c(0);
    // i2c.open_i2c();

    // while(1){
    //     uint8_t buf[7];
    //     i2c.readBytesReg(ADDR,REG,buf,7);
    //     std::cout<< "Receive: ";
    //     for(int i=0;i<7;++i)
    //         std::cout<<int(buf[i])<<" ";
    //     std::cout<<std::endl;
    // }
    spi_module *spi = new spi_module(0,0,0,8000000,8,false);
    spi->open_spi();
    uint8_t tx_buf[2];
    tx_buf[0] = 0x01;
    uint8_t rx_buf[2];

    gpio_module gpio(16,mraa::DIR_OUT);
    gpio.write(1);

    TouchScreen *touch = new TouchScreen;
    touch->open_touchscreen(0);
    while(1){
        // spi->transfer(tx_buf,rx_buf,2);
        std::pair<double,double> pos = touch->get_touch_pos();
        std::cout<<"x:"<<pos.first<<" "<<"y:"<<pos.second<<std::endl;
    }
}