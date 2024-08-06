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
            _rocker.open_rocker();
            break;
        case IMU:
            break;
    }
}

std::pair<double,double> Debugger::get_value(){
    return _rocker.get_position();
}