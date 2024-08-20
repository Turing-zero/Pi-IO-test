#ifndef ST7789_H
#define ST7789_H

#include <mraa/spi.hpp>
#include <mraa/gpio.hpp>
#include <cstdint>

class ST7789 {
public:
    ST7789(mraa::Spi& spi,
           mraa::Gpio& dc_pin,
        //    mraa::Gpio& cs_pin,
           mraa::Gpio& reset_pin,
           uint16_t width,
           uint16_t height);

    void init();
    void fillScreen(uint16_t color);
    void setCursor(uint16_t x, uint16_t y);
    void setTextColor(uint16_t color);
    void setTextSize(uint8_t size);
    void print(const char* text);

private:
    mraa::Spi& spi;
    mraa::Gpio& dc_pin;
    // mraa::Gpio& cs_pin;
    mraa::Gpio& reset_pin;
    uint16_t width;
    uint16_t height;
    uint16_t cursor_x;
    uint16_t cursor_y;
    uint16_t text_color;
    uint8_t text_size;

    void sendCommand(uint8_t cmd);
    void sendData(uint8_t data);
    void reset();
    void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
};

#endif // ST7789_H