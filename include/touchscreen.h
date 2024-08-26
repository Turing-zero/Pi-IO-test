#ifndef __TOUCHSCREEN__
#define __TOUCHSCREEN__
#include "lib_io.h"

class TouchScreen{
public:
    //TouchScreen();
    ~TouchScreen();
    void open_touchscreen(int bus);
    std::pair<double,double> get_touch_pos();
private:
    i2c_module *touch_screen;
};

#endif