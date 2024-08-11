#ifndef __DYNAMIXEL__
#define __DYNAMIXEL__
#include "rpi_lib.h"

class Dynamixel{
  public:
  void open_dynamixel();
  private:
  rs485_module *Dynamixel;  
};
#endif