#include "dynamixel1.0.h"

void Dynamixel_1::open_dynamixel(int baudrate,int uart_delay,Uart_Port port,int control_pin){
    dynamixel = new rs485_module(baudrate,uart_delay,port,control_pin);
    dynamixel->open_rs485();
}

int Dynamixel_1::reboot(int id,char* recv_buf){
    //command
    char command[6] = {0xFF,0xFF,0x00,0x02,0x08};
    command[2] = id;
    int CKSM = 0;
    for(int i = 2;i<5;++i){
        CKSM+=command[i];
    }
    command[5]=~CKSM;

    //action and recv state
    dynamixel->send_485packet(command,6);
    int len = 0;
    while(len==0){
        len=dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}

int Dynamixel_1::factory_reset(int id,char* recv_buf){
    //command
    char command[6] = {0xFF,0xFF,0x00,0x02,0x06};
    command[2] = id;
    int CKSM = 0;
    for(int i = 2;i<5;++i){
        CKSM+=command[i];
    }
    command[5]=~CKSM;

    //action and recv state
    dynamixel->send_485packet(command,6);
    int len = 0;
    while(len==0){
        len=dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}


int Dynamixel_1::ping(int id,char* recv_buf){
    //command
    char command[6] = {0xFF,0xFF,0x00,0x02,0x01};
    command[2] = id;
    int CKSM = 0;
    for(int i = 2;i<5;++i){
        CKSM+=command[i];
    }
    command[5]=~CKSM;

    //action and recv state
    dynamixel->send_485packet(command,6);
    int len = 0;
    while(len==0){
        len=dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}

int Dynamixel_1::read(int id,char* recv_buf,int address,int size){
    //command
    char command[8] = {0xFF,0xFF,0x00,0x04,0x02};
    command[2] = id;
    command[5] = address;
    command[6] = size;
    int CKSM = 0;
    for(int i = 2;i<7;++i){
        CKSM+=command[i];
    }
    command[7]=~CKSM;

    //action and recv state
    dynamixel->send_485packet(command,8);
    int len = 0;
    while(len==0&&id!=0xFE){
        len=dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}

int Dynamixel_1::write(int id,char* recv_buf,int address,int data){
    //command
    char command[9] = {0xFF,0xFF,0x00,0x05,0x03};
    command[2] = id;
    command[5] = address;
    command[6] = data&0xFF;
    command[7] = data>>8&0xFF;
    int CKSM = 0;
    for(int i = 2;i<8;++i){
        CKSM+=command[i];
    }
    command[8]=~CKSM;

    //action and recv state
    dynamixel->send_485packet(command,9);
    int len = 0;
    while(len==0&&id!=0xFE){
        len=dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}

int Dynamixel_1::regwrite(int id,char* recv_buf,int address,int data){
    //command
    char command[9] = {0xFF,0xFF,0x00,0x05,0x04};
    command[2] = id;
    command[5] = address;
    command[6] = data&0xFF;
    command[7] = data>>8&0xFF;
    int CKSM = 0;
    for(int i = 2;i<8;++i){
        CKSM+=command[i];
    }
    command[8]=~CKSM;

    //action and recv state
    dynamixel->send_485packet(command,9);
    int len = 0;
    while(len==0&&id!=0xFE){
        len=dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}

int Dynamixel_1::action(int id,char* recv_buf){
    //command
    char command[6] = {0xFF,0xFF,0x00,0x02,0x05};
    command[2] = id;
    int CKSM = 0;
    for(int i = 2;i<5;++i){
        CKSM+=command[i];
    }
    command[5]=~CKSM;

    //action and recv state
    dynamixel->send_485packet(command,6);
    int len = 0;
    while(len==0&&id!=0xFE){
        len=dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}
/*
TODO 理论上address作为起始地址，后续的地址内容都可以被赋值，举个例子假设address为0x18  0x18控制torque enable的，
占两个字节  理论上传两个字节数据就可以了，假设传了四个字节的数据，那么后续的内容继承的赋值给0x18后续的寄存器，理论上0x18后续的
寄存器都能被赋值，但因为代码data的大小问题，目前只支持address后面8个字节的赋值，后续需要赋值多位寄存器需要修改

此指令用于通过单个指令包传输同时控制多个 DYNAMIXEL。使用此指令时，可以一次传输多个指令，这样当多个 DYNAMIXEL 连接到单个通道时，
通信时间就会减少。但是，同步写入指令只能用于连接的 DYNAMIXEL 上具有相同长度数据的单个地址。ID 应作为广播 ID 传输。
*/
void Dynamixel_1::sync_write(int*id_group,long int*data,int size,int address,int datasize){
    //command
    char command[256] = {0xFF,0xFF,0xFE,0x00,0x83};
    command[5] = address;
    command[6] = datasize;
    int before_id_bit =7;

    for(int i=0;i<size;++i){
        command[before_id_bit++]=id_group[i];
        int addr_mov=0;
        for(int j=0;j<datasize;j++){
            command[before_id_bit++]=(data[i]>>addr_mov)&0xFF;
            addr_mov+=8;
        }
    }
    int CKSM = 0;
    command[3]=before_id_bit-3;
    for(int i = 2;i<before_id_bit;++i){
        CKSM+=command[i];
    }
    command[before_id_bit++]=~CKSM;
    //action and recv state
    dynamixel->send_485packet(command,before_id_bit);
}


int Dynamixel_1::bulk_read(char* recv_buf,int*id_group,int*address,int*data_size,int size){
    //command
    char command[256]={0xFF,0xFF,0xFE,0x00,0x92,0x00};
    int before_id_bit = 6;
    for(int i=0;i<size;++i){
        // command[before_id_bit+(i-before_id_bit)*5]=id_group[i-before_id_bit];
        // command[before_id_bit+(i-before_id_bit)*5+1]=address[i-before_id_bit]&0xFF;
        // command[before_id_bit+(i-before_id_bit)*5+2]=address[i-before_id_bit]>>8&0xFF;
        // command[before_id_bit+(i-before_id_bit)*5+3]=data_size[i-before_id_bit]&0xFF;
        command[before_id_bit++]=data_size[i];
        command[before_id_bit++]=id_group[i];
        command[before_id_bit++]=address[i];
    }
    command[3]=before_id_bit-3;
    int CKSM = 0;
    for(int i = 2;i<before_id_bit;++i){
        CKSM+=command[i];
    }
    command[before_id_bit++]=~CKSM;

    //action and recv state
    dynamixel->send_485packet(command,before_id_bit);
    int len = 0;
    int totallen = 0;
    // int count=0;
    for(int i=0;i<size;++i){
        while(len==0){
            len = dynamixel->recv_485packet(recv_buf+totallen,1024);
        }
        totallen+=len;
        len = 0;
    }
    return totallen;
}

int Dynamixel_1::angle2hex(double angle){
    int step = 0xFFF/(M_PI*2);
    return angle*step;
}

void Dynamixel_1::action_angle(int id,double angle){
    char recv_buf[256]="";
    //打开torque enable 才能控制舵机
    write(id,recv_buf,0x18,0x01);
    regwrite(id,recv_buf,0x1E,angle2hex(angle));
    action(id,recv_buf);
    //关闭torque enable
    // write(id,recv_buf,0x40,0x00);
}
