#include "dynamixel.h"

void Dynamixel::open_dynamixel(int baudrate,int uart_delay,Uart_Port port,int control_pin){
    dynamixel = new rs485_module(baudrate,uart_delay,port,control_pin);
    dynamixel->open_rs485();
}

char* Dynamixel::reboot(int id){
    //command
    char command[] = {0xFF,0xFF,0xFD,0x00,0x02,0x03,0x00,0x08};
    command[4] = id;
    char recv_buf[256]="";
    int before_crc_bits = sizeof(command);
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,before_crc_bits);
    command[before_crc_bits] = CRC_result&0xff;
    command[before_crc_bits+1] = CRC_result>>8&0xff;
    //action and recv state
    dynamixel->send_485packet(command,before_crc_bits+2);
    dynamixel->recv_485packet(recv_buf,256);
    std::cout<<sizeof(recv_buf)<<std::endl;
    return recv_buf;
}

char* Dynamixel::action_angle(int id,double angle){
    // char tx_buf[] = {0xFF,0xFF,0xFD,0x00,0x02,0x09,0x00,0x03,0x40,0x00,0x01,0x00,0x00,0x00};//reg-write
    // char tx_buf_d[] = {0xFF,0xFF,0xFD,0x00,0x02,0x09,0x00,0x04,0x74,0x00,0x00,0x00,0x00,0x00};//reg-write
    // char tx_buf_a[] = {0xFF,0xFF,0xFD,0x00,0x02,0x03,0x00,0x05};//action
}

char* Dynamixel::factory_reset(int id){
    //command
    char command[] = {0xFF,0xFF,0xFD,0x00,0x00,0x04,0x00,0x06,0x01};
    command[4] = id;
    char recv_buf[]={};
    int before_crc_bits = sizeof(command);
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,before_crc_bits);
    command[before_crc_bits] = CRC_result&0xff;
    command[before_crc_bits+1] = CRC_result>>8&0xff;
    
    //action and recv state
    dynamixel->send_485packet(command,before_crc_bits+2);
    dynamixel->recv_485packet(recv_buf,256);
    return recv_buf;
}

/*
检查设备是否存在及基本信息的说明
无论设备的状态返回级别如何，状态包总是发送给 Ping 指令。
当数据包ID字段(command[4])为0xFE（广播ID）时：所有设备按照其排列的顺序发送其状态数据包。
*/
char* Dynamixel::ping(int id){
    //command
    char command[]={0xFF,0xFF,0xFD,0x00,0x00,0x03,0x00,0x01};
    command[4] = id;
    char recv_buf[]={};
    int before_crc_bits = sizeof(command);
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,before_crc_bits);
    command[before_crc_bits] = CRC_result&0xff;
    command[before_crc_bits+1] = CRC_result>>8&0xff;

    //action and recv state
    dynamixel->send_485packet(command,before_crc_bits+2);
    dynamixel->recv_485packet(recv_buf,256);
    return recv_buf;
}

char* Dynamixel::read(int id){
    //command
    char command[]={0xFF,0xFF,0xFD,0x00,0x00,0x07,0x00,0x02,0x84,0x00,0x04,0x00};
    command[4] = id;
    char recv_buf[]={};
    int before_crc_bits = sizeof(command);
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,before_crc_bits);
    command[before_crc_bits] = CRC_result&0xff;
    command[before_crc_bits+1] = CRC_result>>8&0xff;

    //action and recv state
    dynamixel->send_485packet(command,before_crc_bits+2);
    dynamixel->recv_485packet(recv_buf,256);
    return recv_buf;
}

char* Dynamixel::write(int id){
    //command
    char command[]={0xFF,0xFF,0xFD,0x00,0x00,0x09,0x00,0x03,0x74,0x00,0x00,0x02,0x00,0x00};
    command[4] = id;
    char recv_buf[]={};
    int before_crc_bits = sizeof(command);
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,before_crc_bits);
    command[before_crc_bits] = CRC_result&0xff;
    command[before_crc_bits+1] = CRC_result>>8&0xff;

    //action and recv state
    dynamixel->send_485packet(command,before_crc_bits+2);
    dynamixel->recv_485packet(recv_buf,256);
    return recv_buf;
}

// char* Dynamixel::regwrite(int id){
//     //command
//     char command[]={0xFF,0xFF,0xFD,0x00,0x00,0x09,0x00,0x03,0x74,0x00,0x00,0x02,0x00,0x00};
//     command[4] = id;
//     char recv_buf[]={};
//     int before_crc_bits = sizeof(command);
//     unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,before_crc_bits);
//     command[before_crc_bits] = CRC_result&0xff;
//     command[before_crc_bits+1] = CRC_result>>8&0xff;

//     //action and recv state
//     dynamixel->send_485packet(command,before_crc_bits+2);
//     dynamixel->recv_485packet(recv_buf,256);
//     return recv_buf;
// }

