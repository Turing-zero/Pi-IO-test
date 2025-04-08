#ifndef __DYNAMIXEL_1__
#define __DYNAMIXEL_1__
#include "lib_io.h"
#include "proto/MX-64.h"
#include <cmath>

class Dynamixel_1{
    public:
        void open_dynamixel(int baudrate = 1000000,int uart_delay = 400,Uart_Port port = UART0,int control_pin=36);

        void set_goal_position(int id,uint16_t position);
        void set_goal_position_deg(int id, int deg);
        void set_moving_speed(int id, uint16_t speed);
        void set_moving_speed_rpm(int id, int rpm);

        void action_angle(int id,double angle);
        int factory_reset(int id,char* recv_buf);
        int ping(int id,char* recv_buf);
        int read(int id,char* recv_buf,int address,int size);
        int write(int id,char* recv_buf,int address,int data);
        int regwrite(int id,char* recv_buf,int address,int data);
        int action(int id,char* recv_buf);
        int bulk_read(char* recv_buf,int*id_group,int*address,int*data_size,int size);
        void sync_write(int*id_group,long int*data,int size,int address,int datasize);
        int reboot(int id,char* recv_buf);
    private:
        int angle2hex(double angle);
        rs485_module *dynamixel;  
};

#endif