#include "touchscreen.h"

#define READ_REG 0x02
#define ADDRESS 0x15

TouchScreen::~TouchScreen(){
    touch_screen->close_i2c();
    delete touch_screen;
}

void TouchScreen::open_touchscreen(int bus){
    touch_screen = new i2c_module(bus);
    touch_screen->open_i2c();
}

std::pair<double,double> TouchScreen::get_touch_pos(){
    uint8_t buf[7];
    touch_screen->readBytesReg(ADDRESS,READ_REG,buf,7);
    return std::make_pair(buf[2],buf[4]);
}

extern "C"{
    TouchScreen* TouchScreen_py(){
        return new TouchScreen();
    }
    void open_touch_screen_py(TouchScreen*touch_py){
        touch_py->open_touchscreen(0);
    }
    std::pair<double,double> get_touch_pos_py(TouchScreen*touch_py){
        return touch_py->get_touch_pos(); 
    }
    void TouchScreen_delete(TouchScreen *touch_py){
        delete touch_py;
    }
}