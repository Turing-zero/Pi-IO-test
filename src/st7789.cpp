#include "st7789.h"
#include <cstring>
#include <unistd.h>

ST7789::ST7789(mraa::Spi& spi, mraa::Gpio& dc_pin, /*mraa::Gpio& cs_pin,*/ mraa::Gpio& reset_pin, uint16_t width, uint16_t height)
    : spi(spi), dc_pin(dc_pin), /*cs_pin(cs_pin),*/ reset_pin(reset_pin), width(width), height(height), cursor_x(0), cursor_y(0), text_color(0xFFFF), text_size(1) {}

void ST7789:: init() {
    reset();
    // 初始化命令...
    sendCommand(0x11); // 退出睡眠模式
    usleep(120000); // 等待120ms

    sendCommand(0x36);
    sendData(0x00);

    sendCommand(0x3A);
    sendData(0x05);

    sendCommand(0xB2);
    sendData(0x0C);
    sendData(0x0C);
    sendData(0x00);
    sendData(0x33);
    sendData(0x33);

    sendCommand(0xB7);
    sendData(0x35);

    sendCommand(0xBB);
    sendData(0x19);

    sendCommand(0xC0);
    sendData(0x2C);

    sendCommand(0xC2);
    sendData(0x01);

    sendCommand(0xC3);
    sendData(0x12);

    sendCommand(0xC4);
    sendData(0x20);

    sendCommand(0xC6);
    sendData(0x0F);

    sendCommand(0xD0);
    sendData(0xA4);
    sendData(0xA1);

    sendCommand(0xE0);
    sendData(0xD0);
    sendData(0x04);
    sendData(0x0D);
    sendData(0x11);
    sendData(0x13);
    sendData(0x2B);
    sendData(0x3F);
    sendData(0x54);
    sendData(0x4C);
    sendData(0x18);
    sendData(0x0D);
    sendData(0x0B);
    sendData(0x1F);
    sendData(0x23);

    sendCommand(0xE1);
    sendData(0xD0);
    sendData(0x04);
    sendData(0x0C);
    sendData(0x11);
    sendData(0x13);
    sendData(0x2C);
    sendData(0x3F);
    sendData(0x44);
    sendData(0x51);
    sendData(0x2F);
    sendData(0x1F);
    sendData(0x1F);
    sendData(0x20);
    sendData(0x23);

    sendCommand(0x21);

    sendCommand(0x11);
    usleep(120000);

    sendCommand(0x29);
    usleep(120000);
}

void ST7789::fillScreen(uint16_t color) {
    setAddrWindow(0, 0, width - 1, height - 1);
    uint8_t buf[2];
    // uint8_t high_byte = color >> 8;
    // uint8_t low_byte = color & 0xFF;
    buf[0] = color >> 8;
    buf[1] = color & 0xFF;
    dc_pin.write(1);
    for (uint32_t i = 0; i < width * height; i++) {
        // spi.write(&high_byte,1);
        // spi.write(&low_byte,1);
        spi.write(buf,2);
    }
}

void ST7789::setCursor(uint16_t x, uint16_t y) {
    cursor_x = x;
    cursor_y = y;
}

void ST7789::setTextColor(uint16_t color) {
    text_color = color;
}

void ST7789::setTextSize(uint8_t size) {
    text_size = size;
}

void ST7789::print(const char* text) {
    // 打印文本的实现...
}

void ST7789::sendCommand(uint8_t cmd) {
    dc_pin.write(0);
    // cs_pin.write(0);
    spi.write(&cmd,1);
    // cs_pin.write(1);
}

void ST7789::sendData(uint8_t data) {
    dc_pin.write(1);
    // cs_pin.write(0);
    spi.write(&data,1);
    // cs_pin.write(1);
}

void ST7789::reset() {
    reset_pin.write(0);
    usleep(50000); // 50ms
    reset_pin.write(1);
    usleep(50000); // 50ms
}

void ST7789::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    sendCommand(0x2A); // 列地址设置
    // sendData(x0 >> 8);
    // sendData(x0 & 0xFF);
    // sendData(x1 >> 8);
    // sendData(x1 & 0xFF);
    sendData(x0);
    sendData(x1);

    sendCommand(0x2B); // 行地址设置
    // sendData(y0 >> 8);
    // sendData(y0 & 0xFF);
    // sendData(y1 >> 8);
    // sendData(y1 & 0xFF);
    sendData(y0);
    sendData(y1);

    sendCommand(0x2C); // 内存写入
}