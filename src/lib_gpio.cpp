#include "lib_io.h"

gpio_module::gpio_module(int gpio_num,mraa::Dir dir){
    try {
        gpio = new mraa::Gpio(gpio_num);
        gpio->dir(dir);
    } catch (std::exception &e) {
        std::cerr << "Error while setting up raw Gpio, do you have a gpio?" << std::endl;
        std::terminate();
    }
}
gpio_module::~gpio_module(){
    close();
    delete gpio;
}

void gpio_module::write(int value){
    gpio->write(value);
}

int gpio_module::read(){
    return gpio->read();
}

void gpio_module::close(){
    gpio->write(0);
    gpio->close();
}