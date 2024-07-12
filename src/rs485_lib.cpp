#include "rpi_lib.h"

rs485_module::rs485_module(int baudrate,int uart_delay ,Uart_Port port,int databyte,mraa::UartParity parity,int stopbits,bool xonxoff,bool rtscts):uart_module(baudrate,uart_delay,port,databyte,parity,stopbits,xonxoff,rtscts){
    switch(port){
        case UART3:
            _Rs_SwPort = RS485_RW_Switch_Gpio3;
            break;  
        case UART4:
            _Rs_SwPort = RS485_RW_Switch_Gpio4;
            break;  
        case UART5:
            _Rs_SwPort = RS485_RW_Switch_Gpio5; 
            break;  
        default: 
            _Rs_SwPort = Fault_Gpio;
    }
    std::cout << _Rs_SwPort << std::endl;
}

void rs485_module::open_rs485(){
    // init rs485
    uart_module::open_uart();
    try {
        gpio = new mraa::Gpio(_Rs_SwPort);
        gpio->dir(mraa::DIR_OUT);
    } catch (std::exception &e) {
        std::cerr << "Error while setting up raw Gpio, do you have a gpio?" << std::endl;
        std::terminate();
    }
}
rs485_module::~rs485_module(){
    delete gpio;
}

void rs485_module::close_rs485(){
    uart_module::close_uart();
}

void rs485_module::send_485packet(std::string packet){
    gpio->write(1);
    uart_module::send_packet(packet);
    // std::cout << uart_module::getUartDelay() << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(uart_module::getUartDelay()));
}

void rs485_module::recv_485packet(char *packet,int max_size){
    gpio->write(0);
    uart_module::recv_packet(packet,max_size);
}
