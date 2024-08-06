#include "rocker.h"

void Rocker::open_rocker(){
    rocker = new spi_module(0,0,0,500000,8,false);
    rocker->open_spi();
}

double Rocker::get_value(Channel channel){
    uint8_t send_data[BUFFER_SIZE] = {0x00,0x00};
    uint8_t recv_data[BUFFER_SIZE] = {0};
    uint16_t config = 0xF120;
    config |= channel << 9;
    send_data[0] = config >> 8 & 0xFF;
    send_data[1] = config & 0xFF;
    rocker->transfer(send_data,recv_data,2);
    uint16_t result = (recv_data[0]<<8) | recv_data[1];
    double voltage = (result/65535.0)*REF_V;
    return voltage;
}

std::pair<double,double> Rocker::get_position(double r){
    double adc_pitch = get_value(vr1);
    double adc_yaw = get_value(vr2);
    

    double x = (adc_pitch-1.64)/1.64;
    double upstep = r/(3.28 - 1.76);
    double downstep = r/(1.76);
    double y = 0;
    if(adc_yaw>1.76){
        y=0 + (adc_yaw - 1.76)*upstep;
    }else{
        y=-(1.76-adc_yaw)*downstep;
    }
    return std::pair<double,double>{x,y};
}

std::pair<double,double> Rocker::get_adcvalue(){
    double adc_pitch = get_value(vr1);
    double adc_yaw = get_value(vr2);

    return std::pair<double,double>{adc_pitch,adc_yaw};
}