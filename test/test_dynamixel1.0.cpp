#include "dynamixel1.0.h"

int main(){
    Dynamixel_1 device;
    char recv_buf[256]="";
    device.open_dynamixel(1000000,200,UART2,40);
    int len=device.read(1,recv_buf,0x18,0x01);
    for(int i=0;i<len;++i){
        std::cout<<(int)recv_buf[i]<<" ";
    }
    std::cout<<std::endl;
    // device.write(2,recv_buf,0x18,0x01);
    // device.regwrite(2,recv_buf,0x1E,0x0000);
    // device.action(2,recv_buf);
    len=device.read(2,recv_buf,0x18,0x01);
    for(int i=0;i<len;++i){
        std::cout<<(int)recv_buf[i]<<" ";
    }
    std::cout<<std::endl;
    // device.factory_reset(2,recv_buf);
    int id[5]={0x01,0x02};
    long int data[2]={0x01,0x01};

    device.sync_write(id,data,2,0x18,1);
    len=device.read(2,recv_buf,0x18,0x01);
    for(int i=0;i<len;++i){
        std::cout<<(int)recv_buf[i]<<" ";
    }
    std::cout << std::endl;

    int address[2]={0x18,0x18};
    int data_size[2]={0x01,0x01};
    len = device.bulk_read(recv_buf,id,address,data_size,2);
    for(int i=0;i<len;++i){
        std::cout<<(int)recv_buf[i]<<" ";
    }
    std::cout << std::endl;

    device.action_angle(1,3.14);
    return 0;
}