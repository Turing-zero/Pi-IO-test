#ifndef __BUTTON__
#define __BUTTON__
#include "rpi_lib.h"

#define BUFFER_SIZE 2
#define REF_V 4.096

class Button{
public:
    void open_Button(int bus,int cs,int spi_mode,int frequency,int bit_per_word,bool lsb_mode = false);
    bool get_status(Channel channel);
private:
    spi_module *button;
    double sw1_value;
    double sw2_value;
    double sw3_value;
    double sw4_value;
};
#endif