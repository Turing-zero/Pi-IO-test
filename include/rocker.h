#ifndef __ROCKER__
#define __ROCKER__
#include "lib_io.h"

#define BUFFER_SIZE 2
#define REF_V 4.096

class Rocker{
public:
    //Rocker();
    ~Rocker();
    void open_rocker(int bus,int cs,int spi_mode,int frequency,int bit_per_word,bool lsb_mode = false);
    std::pair<double,double> get_position(double r=1);
    std::pair<double,double> get_adcvalue();
    double get_value(Channel channel);
private:
    spi_module *rocker;
    double vr1_value;
    double vr2_value;
    double sw0_value;

};

extern "C"{
    Rocker *Rocker_py();
    double get_value_py(Rocker*rocker_py,Channel channel);
    void Rocker_delete(Rocker *rocker_py);
}
#endif