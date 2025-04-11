#ifndef __DYNAMIXEL_2__
#define __DYNAMIXEL_2__
#include "lib_io.h"
#include "proto/MX-64-2.0.h"
#include <cmath>

class Dynamixel_2{
  public:
  void open_dynamixel(int baudrate = 1000000,int uart_delay = 400,Uart_Port port = UART0,int control_pin=36);
  void reg_write(int id, char* recv_buf, uint16_t address, int data, int len);
  void dir_write(int id, char* recv_buf, uint16_t address, int data, int len);

  void set_goal_position(int id,int32_t position);
  void set_goal_position_deg(int id, int deg);
  void set_goal_vel(int id, int32_t speed);
  void set_goal_vel_rpm(int id, int rpm);
  void set_torque_enable(int id, bool enable);

  int32_t get_present_position(int id,char* recv_buf);
  double get_present_position_deg(int id,char* recv_buf);
  int32_t get_present_speed(int id,char* recv_buf);
  double get_present_speed_rpm(int id,char* recv_buf);

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
  void action_angle(int id,double angle);
  int reboot(int id,char* recv_buf);
  private:
  int angle2hex(double angle);
  rs485_module *dynamixel;
};
#endif