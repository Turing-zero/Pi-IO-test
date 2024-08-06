#include "rpi_lib.h"

#define BUFFER_SIZE 2
#define REF_V 4.096

enum Channel{
    vr1 = 0,
    vr2 = 1,
    sw0 = 2,
    sw1 = 3,
    sw2 = 4,
    sw3 = 5,
    sw4 = 6
};

class Rocker{
public:
    void open_rocker();
    std::pair<double,double> get_position(double r=1);
    std::pair<double,double> get_adcvalue();
    double get_value(Channel channel);
private:
    spi_module *rocker;
    double vr1_value;
    double vr2_value;
    double sw0_value;
    double sw1_value;
    double sw2_value;
    double sw3_value;
    double sw4_value;
};