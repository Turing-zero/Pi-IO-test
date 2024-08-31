#include "sensor_adc.h"

Sensor_ADC::~Sensor_ADC(){
    _i2c->close_i2c();
    delete _i2c;
}

void Sensor_ADC::open_sensor(int _bus){
    _i2c = new i2c_module(_bus);
    _i2c->open_i2c();
}

float Sensor_ADC::get_voltage_mux(int device,int channel){
    uint16_t config = 0x8183;
    u_int8_t addr;
    if(device < 0 ||device >3){
        std::cout<<"Please input deviceId in 0~3"<<std::endl;
        return -1;
    }
    if(channel < 0 ||channel >1){
        std::cout<<"Please input channel in 0~3"<<std::endl;
        return -1;
    }
    switch(device){
        case 0:
            addr = DEVICE_0;
            break;
        case 1:
            addr = DEVICE_1;
            break;
        case 2:
            addr = DEVICE_2;
            break;
        case 3:
            addr = DEVICE_3;
            break;
    }
    uint16_t mux = 0;
    switch(channel){
        case 0:
            mux = CONFIG_MUX_0_1;
            break;
        case 1:
            mux = CONFIG_MUX_2_3;
            break;
    }
    config = (default_config & 0x8FFF) | mux;

    uint8_t send_data[3];
    send_data[0] = SEND_REG;
    send_data[1] = config >> 8;
    send_data[2] = config & 0xFF;
    _i2c->write(addr,send_data,3);

    uint8_t recv_data[2];
    _i2c->readBytesReg(addr,READ_REG,recv_data,2);
    float voltage = calc_voltage(recv_data);
    return voltage;
}

float Sensor_ADC::calc_voltage(uint8_t *rawdata){
    uint16_t result = (rawdata[0] << 8 | rawdata[1]);
    if(result > 32767){
        return (result - 65535)* (6.144 / 32768.0);
    }else return result* (6.144 / 32768.0);
}