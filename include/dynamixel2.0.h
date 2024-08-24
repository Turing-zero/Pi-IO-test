#ifndef __DYNAMIXEL__
#define __DYNAMIXEL__
#include "rpi_lib.h"
#include <cmath>

class Dynamixel{
  public:
  void open_dynamixel(int baudrate = 1000000,int uart_delay = 400,Uart_Port port = UART0,int control_pin=36);
  char* action_angle(int id,double angle);
  int factory_reset(int id,char* recv_buf);
  int ping(int id,char* recv_buf);
  int read(int id,char* recv_buf,int address,int low_size,int high_size);
  int write(int id,char* recv_buf,int address,int data);
  int regwrite(int id,char* recv_buf,int address,int data);
  int action(int id,char* recv_buf);
  int clear(int id,char* recv_buf,bool clear_err);
  int control_table_backup(int id,char* recv_buf,bool backup_restore);//不支持mx64和mx28
  int sync_read(char* recv_buf,int*id_group,int size,int address,int low_size,int high_size);
  int bulk_read(char* recv_buf,int*id_group,int*address,int*low_size,int*high_size,int size);
  void sync_write(int*id_group,long int*data,int size,int address,int low_size,int high_size);
  void bulk_write(int*id_group,long int*data,int*address,int*low_size,int*high_size,int size);
  int reboot(int id,char* recv_buf);
  private:
  int angle2hex(double angle);
  rs485_module *dynamixel;  
};
#endif