#include "dynamixel2.0.h"

int main(){
    Dynamixel_2 device;
    device.open_dynamixel(1000000,200,CUSTOM,40);
    char recv_buf[256]="";
    device.get_present_position(0,recv_buf);
    device.get_present_position_deg(0,recv_buf);
    return 0;
}