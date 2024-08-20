#include "rpi_lib.h"
#include "st7789.h"

int main(){
    mraa::Spi spi(0);
    spi.mode(mraa::SPI_MODE0);
    spi.frequency(8000000);

    // 初始化控制引脚
    mraa::Gpio dc_pin(22);  // GPIO25 (Pin 22)
    dc_pin.dir(mraa::DIR_OUT);
    // mraa::Gpio cs_pin(24);   // GPIO8 (Pin 24)
    // cs_pin.dir(mraa::DIR_OUT);
    mraa::Gpio reset_pin(18);  // GPIO24 (Pin 18)
    reset_pin.dir(mraa::DIR_OUT);

    // 初始化ST7789显示屏
    uint16_t width = 240;  // 设置宽度
    uint16_t height = 240; // 设置高度
    ST7789 display(spi, dc_pin,/* cs_pin,*/ reset_pin, width, height);  // 传递SPI总线、控制引脚和分辨率
    display.init();

    while(1){
        display.fillScreen(0xFF00);
    }
    
    return 0;
}