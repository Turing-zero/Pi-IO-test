#include <mraa/spi.h>
#include <mraa/gpio.h>
#include <unistd.h>
#include <cstring>

// 定义引脚
#define CS_PIN 8
#define RESET_PIN 25
#define DC_PIN 24

// 字体数据
const uint8_t font[96][5] = {
    {0x1F, 0x05, 0x05, 0x1F, 0x00}, // 'A'
    // 其他字符的数据...
};

// 初始化 SPI 和 GPIO
mraa_spi_context spi;
mraa_gpio_context cs, reset, dc;

// 初始化 TFT LCD
void tft_init() {
    mraa_gpio_write(reset, 1);
    usleep(5000);
    mraa_gpio_write(reset, 0);
    usleep(5000);
    mraa_gpio_write(reset, 1);
    usleep(5000);

    // 初始化序列
    // 发送命令和数据
}

void send_command(uint8_t command) {
    mraa_gpio_write(dc, 0);
    mraa_gpio_write(cs, 0);
    mraa_spi_write(spi, command);
    mraa_gpio_write(cs, 1);
}

void send_data(uint8_t data) {
    mraa_gpio_write(dc, 1);
    mraa_gpio_write(cs, 0);
    mraa_spi_write(spi, data);
    mraa_gpio_write(cs, 1);
}

void draw_pixel(int x, int y, uint16_t color) {
    send_command(0x2A);
    send_data(x >> 8);
    send_data(x & 0xFF);
    send_command(0x2B);
    send_data(y >> 8);
    send_data(y & 0xFF);
    send_command(0x2C);
    send_data(color >> 8);
    send_data(color & 0xFF);
}

void draw_char(int x, int y, char c, uint16_t color) {
    if (c < 32 || c > 127) return; // 忽略不可打印字符

    const uint8_t* char_data = font[c - 32];

    for (int i = 0; i < 5; i++) {
        uint8_t line = char_data[i];
        for (int j = 0; j < 7; j++) {
            if (line & 0x01) {
                draw_pixel(x + i, y + j, color);
            }
            line >>= 1;
        }
    }
}

void draw_string(int x, int y, const char* str, uint16_t color) {
    while (*str) {
        draw_char(x, y, *str++, color);
        x += 6; // 移动到下一个字符位置
    }
}

int main() {
    mraa_init();

    spi = mraa_spi_init(0);
    mraa_spi_frequency(spi, 4000000);
    mraa_spi_mode(spi, MRAA_SPI_MODE0);

    cs = mraa_gpio_init(CS_PIN);
    reset = mraa_gpio_init(RESET_PIN);
    dc = mraa_gpio_init(DC_PIN);
    mraa_gpio_dir(cs, MRAA_GPIO_OUT);
    mraa_gpio_dir(reset, MRAA_GPIO_OUT);
    mraa_gpio_dir(dc, MRAA_GPIO_OUT);

    tft_init();

    // 绘制背景
    for (int x = 0; x < 240; x++) {
        for (int y = 0; y < 320; y++) {
            draw_pixel(x, y, 0x0000); // 黑色背景
        }
    }

    // 绘制文本
    draw_string(10, 10, "Hello, World!", 0xFFFF); // 白色文本

    mraa_spi_stop(spi);
    mraa_gpio_close(cs);
    mraa_gpio_close(reset);
    mraa_gpio_close(dc);
    mraa_deinit();

    return 0;
}