#include "lib_io.h"

i2c_module::i2c_module(int bus){
    _bus=bus;
}

i2c_module::~i2c_module(){
    delete i2c;
}

void i2c_module::open_i2c(){
    try {
        i2c = new mraa::I2c(_bus);
    } catch (std::exception &e) {
        std::cerr << "Error while setting up raw i2c, do you have a i2c" << _bus <<"?" <<std::endl;
        std::terminate();
    }
}

mraa::Result i2c_module::write(uint8_t addr,uint8_t*data,int length){
    i2c->address(addr);
    return i2c->write(data,length);
}

int i2c_module::read(uint8_t addr,uint8_t*data,int length){
    i2c->address(addr);
    return i2c->read(data,length);
}

mraa::Result i2c_module::writeByte(uint8_t addr,uint8_t data){
    i2c->address(addr);
    return i2c->writeByte(data);
    
}

uint8_t i2c_module::readByte(uint8_t addr,uint8_t data){
    i2c->address(addr);
    return i2c->readByte();
}

int i2c_module::readBytesReg(uint8_t addr,uint8_t reg,uint8_t *data,int length){
    i2c->address(addr);
    return i2c->readBytesReg(reg,data,length);
}

void i2c_module::close_i2c(){
    i2c->close();
}

extern "C"{
    i2c_module* I2c_py(int bus){
        return new i2c_module(bus);
    }
    void open_i2c_py(i2c_module*i2c_py){
        i2c_py->open_i2c();
    }
    void write_i2c_py(i2c_module*i2c_py,uint8_t addr,uint8_t *data,int length){
        i2c_py->write(addr,data,length);
    }
    int read_i2c_py(i2c_module*i2c_py,uint8_t addr,uint8_t *data,int length){
        return i2c_py->read(addr,data,length);
    }
    int readbytes_i2c_py(i2c_module*i2c_py,uint8_t addr,uint8_t reg,uint8_t *data,int length){
        return i2c_py->readBytesReg(addr,reg,data,length);
    }
    void I2c_delete(i2c_module*i2c_py){
        delete i2c_py;
    }
}
