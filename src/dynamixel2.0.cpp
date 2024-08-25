#include "dynamixel2.0.h"

void Dynamixel_2::open_dynamixel(int baudrate,int uart_delay,Uart_Port port,int control_pin){
    dynamixel = new rs485_module(baudrate,uart_delay,port,control_pin);
    dynamixel->open_rs485();
}

int Dynamixel_2::reboot(int id,char* recv_buf){
    //command
    char command[10] = {0xFF,0xFF,0xFD,0x00,0x02,0x03,0x00,0x08};
    command[4] = id;
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,8);
    command[8] = CRC_result&0xff;
    command[9] = CRC_result>>8&0xff;

    //action and recv state
    dynamixel->send_485packet(command,10);
    int len = 0;
    while(len==0){
        len=dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}

int Dynamixel_2::angle2hex(double angle){
    int step = 0xFFF/(M_PI*2);
    return angle*step;
}

void Dynamixel_2::action_angle(int id,double angle){
    char recv_buf[256]="";
    //打开torque enable 才能控制舵机
    write(id,recv_buf,0x40,0x01);
    regwrite(id,recv_buf,0x74,angle2hex(angle));
    action(id,recv_buf);
    //关闭torque enable
    // write(id,recv_buf,0x40,0x00);
}

int Dynamixel_2::factory_reset(int id,char* recv_buf){
//     //command
    char command[11] = {0xFF,0xFF,0xFD,0x00,0x00,0x04,0x00,0x06,0x01};
    command[4] = id;
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,9);
    command[9] = CRC_result&0xff;
    command[10] = CRC_result>>8&0xff;
    
    //action and recv state
    dynamixel->send_485packet(command,11);
    int len = 0;
    while(len==0){
        len = dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}

/*
检查设备是否存在及基本信息的说明
无论设备的状态返回级别如何，状态包总是发送给 Ping 指令。
当数据包ID字段(command[4])为0xFE（广播ID）时：所有设备按照其排列的顺序发送其状态数据包。
*/
int Dynamixel_2::ping(int id,char* recv_buf){
    //command
    char command[10]={0xFF,0xFF,0xFD,0x00,0x00,0x03,0x00,0x01};
    command[4] = id;
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,8);
    command[8] = CRC_result&0xff;
    command[9] = CRC_result>>8&0xff;

    //action and recv state
    dynamixel->send_485packet(command,10);
    int len = 0;
    while(len==0){
        len = dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}

int Dynamixel_2::read(int id,char* recv_buf,int address,int low_size,int high_size){
    //command
    int low_address = address&0xFF;
    int high_address = address>>8&0xFF;
    char command[14]={0xFF,0xFF,0xFD,0x00,0x00,0x07,0x00,0x02,0x84,0x00,0x04,0x00};
    command[4] = id;
    command[8] = low_address;
    command[9] = high_address;
    command[10] = low_size;
    command[11] = high_size; 
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,12);
    command[12] = CRC_result&0xFF;
    command[13] = CRC_result>>8&0xFF;

    //action and recv state
    dynamixel->send_485packet(command,14);
    int len = 0;
    while(len==0){
        len = dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}
/*
TODO 理论上address作为起始地址，后续的地址内容都可以被赋值，举个例子假设address为0x40  0x40控制torque enable的，
占两个字节  理论上传两个字节数据就可以了，假设传了四个字节的数据，那么后续的内容继承的赋值给0x40后续的寄存器，理论上0x40后续的
寄存器都能被赋值，但因为代码data占用的大小问题，目前只支持address后面4个字节的赋值，后续需要赋值多位寄存器需要修改
*/
int Dynamixel_2::write(int id,char*recv_buf,int address,int data){
    //command
    int low_address = address&0xFF;
    int high_address = address>>8&0xFF; 
    char command[16]={0xFF,0xFF,0xFD,0x00,0x00,0x09,0x00,0x03,0x74,0x00,0x00,0x02,0x00,0x00};
    command[4] = id;
    command[8] = low_address;
    command[9] = high_address;
    command[10] = data&0xFF;
    command[11] = data>>8&0xFF;
    command[12] = data>>16&0xFF;
    command[13] = data>>24&0xFF;
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,14);
    command[14] = CRC_result&0xFF;
    command[15] = CRC_result>>8&0xFF;

    //action and recv state
    dynamixel->send_485packet(command,16);
    int len = 0;
    while(len==0){
        len = dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}
/*
TODO 理论上address作为起始地址，后续的地址内容都可以被赋值，举个例子假设address为0x40  0x40控制torque enable的，
占两个字节  理论上传两个字节数据就可以了，假设传了四个字节的数据，那么后续的内容继承的赋值给0x40后续的寄存器，理论上0x40后续的
寄存器都能被赋值，但因为代码data占用的大小问题，目前只支持address后面4个字节的赋值，后续需要赋值多位寄存器需要修改

注意:测试结果来看regwrite把数据写入舵机不会立即执行，要配合action指令一起使用
与写指令类似，但同步特性得到改善的指令
当收到指令包时，立即执行写入指令。
通过使用Reg Write和Action指令，可以同时操作多个设备。
注册写指令将指令包注册为待机状态，并将控制表注册指令设置为‘1’。
当收到动作指令时，执行注册的数据包，并将控制表注册指令设置为‘0’。
*/
int Dynamixel_2::regwrite(int id,char*recv_buf,int address,int data){
    //command
    int low_address = address&0xFF;
    int high_address = address>>8&0xFF; 
    char command[16]={0xFF,0xFF,0xFD,0x00,0x00,0x09,0x00,0x04,0x74,0x00,0x00,0x02,0x00,0x00};
    command[4] = id;
    command[8] = low_address;
    command[9] = high_address;
    command[10] = data&0xFF;
    command[11] = data>>8&0xFF;
    command[12] = data>>16&0xFF;
    command[13] = data>>24&0xFF;
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,14);
    command[14] = CRC_result&0xFF;
    command[15] = CRC_result>>8&0xFF;

    //action and recv state
    dynamixel->send_485packet(command,16);
    int len = 0;
    while(len==0){
        len = dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}
/*
执行已使用 Reg Write 指令注册的数据包的指令
当使用写指令控制多个设备时，第一个接收到数据包的设备和最后一个接收到数据包的设备的执行时间会存在差异。
通过使用Reg Write和Action指令，可以同时操作多个设备。
*/
int Dynamixel_2::action(int id,char* recv_buf){
    //command
    char command[10]={0xFF,0xFF,0xFD,0x00,0x00,0x03,0x00,0x05};
    command[4] = id;
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,8);
    command[8] = CRC_result&0xFF;
    command[9] = CRC_result>>8&0xFF;

    //action and recv state
    dynamixel->send_485packet(command,10);
    int len = 0;
    while(len==0){
        len = dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}

/*
默认功能是将当前位置重置为一圈内的绝对值(0-4095)
打开clear_err功能变成清除dynamixel舵机内发生的错误
清除指令仅在 DYNAMIXEL 停止时才适用
如果无法清除错误或不满足清除条件，则错误仍未清除，状态包的错误字段中将显示结果失败 (0x01)
*/
int Dynamixel_2::clear(int id,char* recv_buf,bool clear_err){
    //command
    char command[15]={0xFF,0xFF,0xFD,0x00,0x00,0x08,0x00,0x10,0x01,0x44,0x58,0x4C,0x22};
    command[4]=id;
    if(clear_err){
        command[8]=0x02;
        command[9]=0x45;
        command[10]=0x52;
        command[11]=0x43;
        command[12]=0x4C;
    }
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,13);
    command[13] = CRC_result&0xFF;
    command[14] = CRC_result>>8&0xFF;
    //action and recv state
    dynamixel->send_485packet(command,15);
    int len = 0;
    while(len==0){
        len = dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}
/*
将当前控制表状态数据存储到备份区域或恢复 EEPROM 数据的指令。
仅当 RAM 区域中的扭矩启用设置为 '0'（扭矩关闭状态）时，控制表备份才能正常工作。如果扭矩启用设置为 '1'（扭矩开启），将返回结果失败的状态包。
注意：支持的 DYNAMIXEL：X430/540 系列（Fw v45 或以上）、X330 系列（Fw v46 或以上）、P 系列（Fw v12 或以上）。
控制表中可用于数据备份的项目：
EERPOM中的所有数据
速度 PI 增益
位置 PID 增益
前馈第一增益和第二增益
个人资料加速
剖面速度
间接地址（DYNAMIXEL-P 系列除外）
*/
int Dynamixel_2::control_table_backup(int id,char* recv_buf,bool backup_restore){
    //command
    char command[15]={0xFF,0xFF,0xFD,0x00,0x00,0x08,0x00,0x20,0x01,0x43,0x54,0x52,0x4C};
    command[4]=id;
    if(backup_restore) command[8]=0x02;
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,13);
    command[13] = CRC_result&0xFF;
    command[14] = CRC_result>>8&0xFF;

    //action and recv state
    dynamixel->send_485packet(command,15);
    int len = 0;
    while(len==0){
        len = dynamixel->recv_485packet(recv_buf,25);
    }
    return len;
}

/*
使用一个指令包同时从多个设备读取数据的指令
数据的地址和数据长度必须全部相同。
如果数据的地址不连续，可以使用间接地址。
状态包将根据指令包中的输入ID按顺序返回。
数据包 ID 字段：0xFE（广播 ID）
目前所有同步读取的舵机数据都从recv_buf一个数组内返回，未进行解包
id_group是存储所有需要读取的舵机的id，size是读取舵机的数量，address是需要读取数据的地址，low_size是读取的低位地址数据的字节数。high_size是读取的高位地址数据的字节数。
*/
int Dynamixel_2::sync_read(char* recv_buf,int*id_group,int size,int address,int low_size,int high_size){
    //command
    int high_address = address>>8&0xFF;
    int low_address = address&0xFF;
    char command[256]={0xFF,0xFF,0xFD,0x00,0xFE,0x09,0x00,0x82,0x84,0x00,0x04,0x00};
    int before_id_bit = 12;
    for(int i=before_id_bit;i<before_id_bit+size;++i){
        command[i]=id_group[i-before_id_bit];
    }
    command[5]=0x07+size;
    command[8]=low_address;
    command[9]=high_address;
    command[10]=low_size;
    command[11]=high_size;
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,before_id_bit+size);
    command[before_id_bit+size] = CRC_result&0xFF;
    command[before_id_bit+size+1] = CRC_result>>8&0xFF;

    //action and recv state
    dynamixel->send_485packet(command,before_id_bit+size+2);
    int len = 0;
    int totallen = 0;
    int count=0;
    for(int i=0;i<size;++i){
        while(len==0){
            len = dynamixel->recv_485packet(recv_buf+totallen,1024);
        }
        totallen+=len;
        len = 0;
    }
    return totallen;
}

/*
TODO 理论上address作为起始地址，后续的地址内容都可以被赋值，举个例子假设address为0x40  0x40控制torque enable的，
占两个字节  理论上传两个字节数据就可以了，假设传了四个字节的数据，那么后续的内容继承的赋值给0x40后续的寄存器，理论上0x40后续的
寄存器都能被赋值，但因为代码data占用的大小问题，目前只支持address后面8个字节的赋值，后续需要赋值多位寄存器需要修改

使用一个指令包同时控制多个设备的指令
数据的地址和数据长度必须全部相同。
如果数据的地址不连续，可以使用间接地址。
数据包 ID 字段：0xFE（广播 ID）
id_group是存需要操作舵机的id，data是需要写入的数据，size是有几个舵机需要操作，address是操作地址，lowsize是数据低位的字节，hignsize是数据高位字节
*/

void Dynamixel_2::sync_write(int*id_group,long int*data,int size,int address,int low_size,int high_size){
    //command
    int high_address = address>>8&0xFF;
    int low_address = address&0xFF;
    char command[256]={0xFF,0xFF,0xFD,0x00,0xFE,0x09,0x00,0x83,0x00,0x00,0x00,0x00};
    int before_id_bit = 12;
    command[8]=low_address;
    command[9]=high_address;
    command[10]=low_size;
    command[11]=high_size;
    for(int i=0;i<size;++i){
        command[before_id_bit++]=id_group[i];
        int addr_mov = 0;
        for(int j=0;j<low_size;++j){
            command[before_id_bit++]=(data[i]>>addr_mov&0xFF);
            addr_mov+=8;
        }
        addr_mov=0;
        for(int m=0;m<high_size;++m){
            command[before_id_bit++]=(data[i]>>(addr_mov+32)&0xFF);
            addr_mov+=8;
        }
    }
    command[5]=before_id_bit-7+2;
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,before_id_bit);
    command[before_id_bit++] = CRC_result&0xFF;
    command[before_id_bit++] = CRC_result>>8&0xFF;

    //action and recv state
    dynamixel->send_485packet(command,before_id_bit);
}

/*
与同步读取类似，这是使用一个指令包同时从多个设备读取数据的指令
即使每个设备的数据地址和数据长度不完全相同，也可以使用该指令。
参数中不能多次使用相同的 ID。换句话说，它只能从每个单独的设备读取一次。
状态包将根据指令包中的输入ID按顺序返回。
如果数据的地址不连续，可以使用间接地址。
数据包 ID 字段：0xFE（广播 ID）
id_group是存需要操作舵机的id，size是有几个舵机需要操作，address是操作地址，lowsize是数据低位的字节，hignsize是数据高位字节，recv_buf是存接收到的信息
*/
int Dynamixel_2::bulk_read(char* recv_buf,int*id_group,int*address,int*low_size,int*high_size,int size){
    //command
    char command[256]={0xFF,0xFF,0xFD,0x00,0xFE,0x09,0x00,0x92};
    int before_id_bit = 8;
    for(int i=before_id_bit;i<before_id_bit+size;++i){
        command[before_id_bit+(i-before_id_bit)*5]=id_group[i-before_id_bit];
        command[before_id_bit+(i-before_id_bit)*5+1]=address[i-before_id_bit]&0xFF;
        command[before_id_bit+(i-before_id_bit)*5+2]=address[i-before_id_bit]>>8&0xFF;
        command[before_id_bit+(i-before_id_bit)*5+3]=low_size[i-before_id_bit]&0xFF;
        command[before_id_bit+(i-before_id_bit)*5+4]=high_size[i-before_id_bit]&0xFF;
    }
    command[5]=0x03+size*5;
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,before_id_bit+size*5);
    command[before_id_bit+size*5] = CRC_result&0xFF;
    command[before_id_bit+size*5+1] = CRC_result>>8&0xFF;

    //action and recv state
    dynamixel->send_485packet(command,before_id_bit+size*5+2);
    int len = 0;
    int totallen = 0;
    int count=0;
    for(int i=0;i<size;++i){
        while(len==0){
            len = dynamixel->recv_485packet(recv_buf+totallen,1024);
        }
        totallen+=len;
        len = 0;
    }
    return totallen;
}

/*
TODO 理论上address作为起始地址，后续的地址内容都可以被赋值，举个例子假设address为0x40  0x40控制torque enable的，
占两个字节  理论上传两个字节数据就可以了，假设传了四个字节的数据，那么后续的内容继承的赋值给0x40后续的寄存器，理论上0x40后续的
寄存器都能被赋值，但因为代码data占用的大小问题，目前只支持address后面8个字节的赋值，后续需要赋值多位寄存器需要修改

与同步写入类似，这是使用一个指令包同时控制多个设备的指令
即使每个设备的数据地址和数据长度不完全相同，也可以使用该指令。
参数中不能多次使用同一个ID。也就是说，每个单独的设备只能写入一次。
如果数据的地址不连续，可以使用间接地址。
数据包 ID 字段：0xFE（广播 ID）
id_group是存需要操作舵机的id，data是需要写入的数据，size是有几个舵机需要操作，address是操作地址，lowsize是数据低位的字节，hignsize是数据高位字节
*/
void Dynamixel_2::bulk_write(int*id_group,long int*data,int*address,int*low_size,int*high_size,int size){
    //command
    char command[256]={0xFF,0xFF,0xFD,0x00,0xFE,0x09,0x00,0x93};
    int before_id_bit = 8;
    for(int i=0;i<size;++i){
        command[before_id_bit++]=id_group[i];
        int addr_mov = 0;
        command[before_id_bit++]=address[i]&0xFF;
        command[before_id_bit++]=address[i]>>8&0xFF;
        command[before_id_bit++]=low_size[i];
        command[before_id_bit++]=high_size[i];
        for(int j=0;j<low_size[i];++j){
            command[before_id_bit++]=(data[i]>>addr_mov&0xFF);
            addr_mov+=8;
        }
        addr_mov=0;
        for(int m=0;m<high_size[i];++m){
            command[before_id_bit++]=(data[i]>>(addr_mov+32)&0xFF);
            addr_mov+=8;
        }
    }
    command[5]=before_id_bit-7+2;
    unsigned short CRC_result = dynamixel->update_crc(0,(unsigned char*)command,before_id_bit);
    command[before_id_bit++] = CRC_result&0xFF;
    command[before_id_bit++] = CRC_result>>8&0xFF;

    //action and recv state
    dynamixel->send_485packet(command,before_id_bit);
}
