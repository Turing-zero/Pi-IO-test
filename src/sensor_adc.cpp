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
    if(device < 0 ||device >3){
        std::cout<<"Please input deviceId in 0~3"<<std::endl;
        return -1;
    }
    if(channel < 0 ||channel >1){
        std::cout<<"Please input channel in 0~3"<<std::endl;
        return -1;
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
    uint16_t config = (default_config & 0x8FFF) | mux;
    write_config(device,config);

    uint8_t recv_data[2];
    _i2c->readBytesReg(get_device_addr(device), READ_REG, recv_data, 2);
    float voltage = calc_voltage(recv_data);
    return voltage;
}

void Sensor_ADC::config_data_rate(int device, ADS1115_DATA_RATE rate) {
    uint16_t config = (default_config & 0xFF1F) | ((uint8_t)rate << 5);
    write_config(device, config);
}

void Sensor_ADC::config_pga(int device, ADS1115_PGA pga) {
    uint16_t config = (default_config & 0xF1FF) | ((uint8_t)pga << 9);
    write_config(device, config);
}

uint8_t Sensor_ADC::get_device_addr(int device) {
    uint8_t addr;
    switch (device) {
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
    return addr;
}

void Sensor_ADC::write_config(int device, uint16_t config) {
    uint8_t addr = get_device_addr(device);
    uint8_t send_data[3];
    send_data[0] = SEND_REG;
    send_data[1] = config >> 8;
    send_data[2] = config & 0xFF;
    _i2c->write(addr, send_data, 3);
}

float Sensor_ADC::calc_voltage(uint8_t *rawdata) {
    uint16_t result = (rawdata[0] << 8 | rawdata[1]);
    if (result > 32767) {
        return (result - 65535) * (6.144 / 32768.0);
    } else
        return result * (6.144 / 32768.0);
}