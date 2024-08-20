#include "display.h"

Display::~Display(){
    close();
    delete _reset;
    delete _ds;
    delete _spi;
}

void Display::open_screen(int bus,int cs,int spimode,int frequency,int bitPerWord,bool lsbmode,int reset,int ds){
    _spi = new spi_module(bus,cs,spimode,frequency,bitPerWord,lsbmode);
    _reset = new gpio_module(reset,mraa::DIR_OUT);
    _ds = new gpio_module(ds,mraa::DIR_OUT);
    _spi->open_spi();
}

void Display::write(uint8_t*tx_buf,int length){
    _spi->write(tx_buf,length);
}

void Display::set_resetio(int value){
    _reset->write(value);
}

void Display::set_dsio(int value){
    _ds->write(value);
}

void Display::close(){
    _reset->close();
    _ds->close();
    _spi->close_spi();
}

extern "C"{
    Display* Display_py(){
        return new Display();
    }
    void open_display_py(Display*display_py,int bus,int cs,int spi_mode,int frequency,int bit_per_word,bool lsb_mode,int reset,int ds){
        display_py->open_screen(bus,cs,spi_mode,frequency,bit_per_word,lsb_mode,reset,ds);
    }
    void write_py(Display*display_py,uint8_t *data,int length){
        return display_py->write(data,length);
    }
    void set_resetio_py(Display*display_py,int value){
        return display_py->set_resetio(value);
    }
    void set_dsio_py(Display*display_py,int value){
        return display_py->set_dsio(value);
    }
    void Display_delete(Display*display_py){
        delete display_py;
    }
}