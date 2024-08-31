#ifndef __SENSOR_ADC__
#define __SENSOR_ADC__
#include "lib_io.h"

// 寄存器地址
#define READ_REG 0x00
#define SEND_REG 0x01

#define DEVICE_0 0x48
#define DEVICE_1 0x49
#define DEVICE_2 0x4a
#define DEVICE_3 0x4b

// Config Register
// TODO: full support
#define CONFIG_OS_SINGLE        ((uint8_t)1 << 15)

#define CONFIG_MUX_0_1          ((uint8_t)0 << 12)
#define CONFIG_MUX_2_3          ((uint8_t)3 << 12)
#define CONFIG_MUX_SINGLE_0     ((uint8_t)4 << 12) // 单端输入，AIN0
#define CONFIG_MUX_SINGLE_1     ((uint8_t)5 << 12)
#define CONFIG_MUX_SINGLE_2     ((uint8_t)6 << 12)
#define CONFIG_MUX_SINGLE_3     ((uint8_t)7 << 12)

#define CONFIG_PGA_6_144V       ((uint8_t)0 << 9)  // ±6.144V 全量程

#define CONFIG_MODE_SINGLE      ((uint8_t)1 << 8) // 单次转换模式

#define CONFIG_DR_128SPS        ((uint8_t)4 << 5)
#define CONFIG_DR_860SPS        ((uint8_t)7 << 5)

#define CONFIG_COMP_QUE_DISABLE ((uint8_t)3) // 禁用比较器

class Sensor_ADC{
    public:
        ~Sensor_ADC();
        void open_sensor(int _bus);
        float get_voltage_mux(int device,int channel);
    private:
        float calc_voltage(uint8_t *rawdata);
        i2c_module *_i2c;
        uint16_t default_config = CONFIG_OS_SINGLE | CONFIG_MUX_0_1 | CONFIG_PGA_6_144V | CONFIG_MODE_SINGLE | CONFIG_DR_128SPS | CONFIG_COMP_QUE_DISABLE;
};
#endif