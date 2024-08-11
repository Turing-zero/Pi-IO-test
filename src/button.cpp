#include "button.h"

void Button::open_Button(int bus,int cs,int spi_mode,int frequency,int bit_per_word,bool lsb_mode){
    button = new spi_module(bus,cs,spi_mode,frequency,bit_per_word,lsb_mode);
    button->open_spi();
}

bool Button::get_status(Channel channel){
    uint8_t send_data[BUFFER_SIZE] = {0x00,0x00};
    uint8_t recv_data[BUFFER_SIZE] = {0};
    uint16_t config = 0xF120;
    config |= channel << 9;
    send_data[0] = config >> 8 & 0xFF;
    send_data[1] = config & 0xFF;
    button->transfer(send_data,recv_data,2);
    uint16_t result = (recv_data[0]<<8) | recv_data[1];
    double voltage = (result/65535.0)*REF_V;
    if(voltage > 2) return false;
    return true;
}

extern "C"{
    Button* Button_py(){
        return new Button();
    }
    void open_button_py(Button*button_py,int bus,int cs,int spi_mode,int frequency,int bit_per_word,bool lsb_mode){
        button_py->open_Button(bus,cs,spi_mode,frequency,bit_per_word,lsb_mode);
    }
    double get_status_py(Button*button_py,Channel channel){
        return button_py->get_status(channel);
    }
    void Button_delete(Button *button_py){
        delete button_py;
    }
}