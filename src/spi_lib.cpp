#include "rpi_lib.h"

spi_module::spi_module(int bus,int cs,int spimode,int frequency,int bitPerWord,bool lsbmode){
    _bus=bus;
    _cs=cs;
    _spimode=spimode;
    _frequency=frequency;
    _bitPerWord=bitPerWord;
    _lsbmode=lsbmode;
}

void spi_module::open_spi(){
    // init spi
    try {
        spi = new mraa::Spi(_bus,_cs);
    } catch (std::exception &e) {
        std::cerr << "Error while setting up raw SPI, do you have a spi" << _bus<<"?" <<std::endl;
        std::terminate();
    }
    if (spi->mode(get_mode(_spimode)) != mraa::SUCCESS) {
        std::cerr << "Error setting mode on SPI" << _bus << std::endl;
    }
    if (spi->frequency(_frequency) != mraa::SUCCESS) {
        std::cerr << "Error setting frequency on SPI" <<_bus<< std::endl;
    }
    if (spi->bitPerWord(_bitPerWord) != mraa::SUCCESS) {
        std::cerr << "Error setting bitPerWord on SPI" <<_bus<< std::endl;
    }
    if (spi->lsbmode(_lsbmode) != mraa::SUCCESS) {
        std::cerr << "Error setting lsbmode on SPI" <<_bus<< std::endl;
    }
}
spi_module::~spi_module(){
    delete spi;
}

void spi_module::close_spi(){
    spi->close();
}

mraa::Spi_Mode spi_module::get_mode(int spimode){
    switch (spimode){
    case 0:
        return mraa::SPI_MODE0;
        break;
    case 1:
        return mraa::SPI_MODE1;
        break;
    case 2:
        return mraa::SPI_MODE2;
        break;
    case 3:
        return mraa::SPI_MODE3;
        break;
    default:
        return mraa::SPI_MODE0;
        break;
    }
}

void spi_module::transfer(uint8_t* tx_buf,uint8_t* rx_buf,int length){
    spi->transfer(tx_buf,rx_buf,length);
}

void spi_module::transfer_word(uint16_t* tx_buf,uint16_t* rx_buf,int length){
    spi->transfer_word(tx_buf,rx_buf,length);
}