#include "rocker.h"

Rocker::~Rocker(){
    rocker->close_spi();
    delete rocker;
}

void Rocker::open_rocker(int bus,int cs,int spi_mode,int frequency,int bit_per_word,bool lsb_mode){
    rocker = new spi_module(bus,cs,spi_mode,frequency,bit_per_word,lsb_mode);
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
    

    double x = 0;
    double y = 0;
    double x_upstep = r/(3.28 - 1.64);
    double x_downstep = r/(1.64-0.28);
    double y_upstep = r/(3.28 - 1.76);
    double y_downstep = r/(1.76-0.28);
    if(adc_pitch>1.64){
        x = 0 + (adc_pitch - 1.64)*x_upstep;
    }else{
        if(adc_pitch<0.28) x = -r;
        else x=-(1.64-adc_pitch)*x_downstep;
    }
    if(adc_yaw>1.76){
        y=0 + (adc_yaw - 1.76)*y_upstep;
    }else{
        if(adc_yaw<0.28) y = -r;
        else y=-(1.76-adc_yaw)*y_downstep;
    }
    if(x>r) x = r;
    if(y>r) y = r;
    return std::make_pair(x,y);
}

std::pair<double,double> Rocker::get_adcvalue(){
    double adc_pitch = get_value(vr1);
    double adc_yaw = get_value(vr2);

    return std::pair<double,double>{adc_pitch,adc_yaw};
}

extern "C"{
    Rocker* Rocker_py(){
        return new Rocker();
    }
    void open_rocker_py(Rocker*rocker_py,int bus,int cs,int spi_mode,int frequency,int bit_per_word,bool lsb_mode){
        rocker_py->open_rocker(bus,cs,spi_mode,frequency,bit_per_word,lsb_mode);
    }
    double get_value_py(Rocker*rocker_py,Channel channel){
        return rocker_py->get_value(channel);
    }
    std::pair<double,double> get_position_py(Rocker*rocker_py,double r){
        return rocker_py->get_position(r); 
    }
    std::pair<double,double> get_adcvalue(Rocker*rocker_py){
        return rocker_py->get_adcvalue();
    }
    void Rocker_delete(Rocker *rocker_py){
        delete rocker_py;
    }
}