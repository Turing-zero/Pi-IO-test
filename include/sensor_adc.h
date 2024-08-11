#ifndef __SENSOR_ADC__
#define __SENSOR_ADC__
#include "rpi_lib.h"

// 寄存器地址
#define READ_REG 0x00
#define SEND_REG 0x01

#define DEVICE_0 0x48
#define DEVICE_1 0x49
#define DEVICE_2 0x4a
#define DEVICE_3 0x4b

class Sensor_ADC{
    public:
        ~Sensor_ADC();
        void open_sensor(int _bus);
        float get_voltage(int device,int channel);
    private:
        float calc_voltage(uint8_t *rawdata);
        i2c_module *_i2c;
};
#endif