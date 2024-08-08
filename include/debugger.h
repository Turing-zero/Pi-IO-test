#include "rpi_lib.h"
#include "rocker.h"
#include "button.h"

enum Device{
    DYNAMIXEL = 0,
    ROCKER = 1,
    IMU = 2,
    BUTTON = 3
};


class Debugger{
    public:
        Debugger();
        ~Debugger();
        void open_device(Device device);
        std::pair<double,double> get_value();
        bool get_status(Channel channel);
    private:
        Device _device;
        // rs485_module dynamixel;
        Rocker _rocker;
        Button _button;
};