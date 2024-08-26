#include "mraa.hpp"
#include <iostream>
#include <unistd.h>
#include "lib_io.h"
#include "sensor_adc.h"

#define ADS1115_ADDRESS 0x48

// 寄存器地址
#define ADS1115_CONVERSION_REG 0x00
#define ADS1115_CONFIG_REG     0x01

// 配置寄存器的设置
#define CONFIG_OS_SINGLE      0x8000  // 开始单次转换
#define CONFIG_MUX_SINGLE_0   0x4000  // 单端输入，AIN0
#define CONFIG_PGA_4_096V     0x0000  // ±6.144V 全量程
#define CONFIG_MODE_SINGLE    0x0100  // 单次转换模式
#define CONFIG_DR_128SPS      0x0080  // 128SPS,每秒采样个数
#define CONFIG_COMP_QUE_DISABLE 0x0003  // 禁用比较器

void writeRegister(mraa::I2c* i2c, uint8_t reg, uint16_t value) {
    uint8_t buffer[3];
    buffer[0] = reg;
    buffer[1] = value >> 8;
    buffer[2] = value & 0xFF;
    i2c->address(ADS1115_ADDRESS);
    i2c->write(buffer, 3);
}

uint16_t readRegister(mraa::I2c* i2c, uint8_t reg) {
    uint8_t buffer[2];
    i2c->address(ADS1115_ADDRESS);
    i2c->readBytesReg(reg,buffer,2);
    return (buffer[0] << 8) | buffer[1];
}

float get_voltage(uint16_t voltage){
    if(voltage > 32767){
        return (voltage - 65535)* (6.144 / 32768.0);
    }else return voltage* (6.144 / 32768.0);
}

int main() {
    // 配置 ADS1115
    uint16_t config = CONFIG_OS_SINGLE |
                      CONFIG_MUX_SINGLE_0 |
                      CONFIG_PGA_4_096V |
                      CONFIG_MODE_SINGLE |
                      CONFIG_DR_128SPS |
                      CONFIG_COMP_QUE_DISABLE;

    Sensor_ADC sensor;
    sensor.open_sensor(0);

    uint8_t buffer[3];
    uint8_t buf[2];
    buffer[0] = ADS1115_CONFIG_REG;
    buffer[1] = config >> 8;
    buffer[2] = config & 0xFF;
    int count = 0;
    double cnt = 0;
    while(1){
        float voltage = sensor.get_voltage(0,0);
        count++;
        cnt+=abs(voltage - 1);
        if(count==1000){
            cnt = 0;
            count=0;
        }
    }
    return 0;
}