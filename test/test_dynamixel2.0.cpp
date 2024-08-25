#include "dynamixel2.0.h"

int main(){
    Dynamixel_2 device;
    device.open_dynamixel(1000000,200,UART2,40);
    char recv_buf[256]="";
    // int len= device.reboot(1,recv_buf);
    // int len= device.factory_reset(1,recv_buf);
    
    int len = device.read(1,recv_buf,0x74,4,0);
    // std::cout<<"first:";
    // for(int i=0;i<len;++i){
    //     std::cout<<(int)recv_buf[i]<<" ";
    // }
    // std::cout<<std::endl;
    // device.write(1,recv_buf,0x40,0x01);
    // len=device.read(1,recv_buf,0x40,4,0);
    // for(int i=0;i<len;++i){
    //     std::cout<<(int)recv_buf[i]<<" ";
    // }
    // std::cout<<std::endl;
    // device.regwrite(1,recv_buf,0x74,0x8F0);
    // device.action(1,recv_buf);
    // len = device.read(1,recv_buf,0x74,4,0);
    // std::cout<<"second:";
    // for(int i=0;i<len;++i){
    //     std::cout<<(int)recv_buf[i]<<" ";
    // }
    // std::cout<<std::endl;
    // std::cout<<"third:";
    int id[5] = {0x01,0x02};
    // len = device.sync_read(recv_buf,id,2,0x84,4,0);
    // len = device.clear(1,recv_buf,true);
    //  for(int i=0;i<len;++i){
    //     std::cout<<(int)recv_buf[i]<<" ";
    // }
    // std::cout<<std::endl;
    // int data[2]={0xFF0,0xFF0};
    long int sw[2]={0x01,0x01};
    device.sync_write(id,sw,1,0x40,4,0);
    // device.sync_write(id,data,2,0x74,4,0);
    int address[2]={0x40,0x40};
    int low_size[2]={0x01,0x01};
    int high_size[2]={0x00,0x00};
    long int data[2]={0x00,0x00};
    // device.bulk_write(id,data,address,low_size,high_size,2);
    // device.write(1,recv_buf,0x40,0x00);
    // len=device.bulk_read(recv_buf,id,address,low_size,high_size,2);
    // for(int i=0;i<len;++i){
    //     std::cout<<(int)recv_buf[i]<<" ";
    // }
    // std::cout<<std::endl;
    device.action_angle(1,M_PI*1);
    
    // std::cout<<len<<std::endl;
    return 0;
}