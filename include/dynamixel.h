#ifndef __DYNAMIXEL__
#define __DYNAMIXEL__
#include "rpi_lib.h"

class Dynamixel{
  public:
  void open_dynamixel(int baudrate = 1000000,int uart_delay = 400,Uart_Port port = UART0,int control_pin=36);
  char* action_angle(int id,double angle);
  char* factory_reset(int id);
  char* ping(int id);
  char* read(int id);
  char* write(int id);
  char* regwrite(int id);
  char* reboot(int id);
  private:
  rs485_module *dynamixel;  
};
#endif