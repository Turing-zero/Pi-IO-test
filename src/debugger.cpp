#include "debugger.h"

Debugger::Debugger(){

}

Debugger::~Debugger(){

}

void Debugger::open_device(Device device){
    _device = device;
    switch(device){
        case DYNAMIXEL:
            break;
        case ROCKER:
            _rocker.open_rocker(0,0,0,500000,8,false);
            break;
        case IMU:
            break;
        case BUTTON:
            _button.open_Button(0,0,0,500000,8,false);
            break;
    }
}

std::pair<double,double> Debugger::get_value(){
    return _rocker.get_position();
    // return _rocker.get_adcvalue();
}

bool Debugger::get_status(Channel channel){
    return _button.get_status(channel);
}