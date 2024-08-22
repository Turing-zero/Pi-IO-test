#include "dynamixel.h"

int main(){
    Dynamixel device;
    device.open_dynamixel(1000000,200,UART2,40);
    char* recv= device.reboot(1);
    // for(int i=0;i<sizeof(recv);++i){
    //     std::cout<<(int)recv[i]<<" ";
    // }
    // std::cout<<std::endl;
    std::cout<<sizeof(recv)<<std::endl;
    return 0;
}