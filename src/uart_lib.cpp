#include "rpi_lib.h"
#include "time.h"

uart_module::uart_module(int baudrate ,Uart_Port port,int databyte,mraa::UartParity parity,int stopbits,bool xonxoff,bool rtscts){
    _baudrate = baudrate;
    _port = port;
    _databyte=databyte;
    _parity=parity;
    _stopbyte=stopbits;
    _xonxoff=xonxoff;
    _rtscts=rtscts;
}

void uart_module::open_uart(){
    // init uart
    try {
        uart = new mraa::Uart(devices::dev_path[_port]);
    } catch (std::exception &e) {
        std::cerr << "Error while setting up raw UART, do you have a uart?" << std::endl;
        std::terminate();
    }
    if (uart->setBaudRate(_baudrate) != mraa::SUCCESS) {
        std::cerr << "Error setting parity on UART" << std::endl;
    }
    if (uart->setMode(_databyte, _parity, _stopbyte) != mraa::SUCCESS) {
        std::cerr << "Error setting parity on UART" << std::endl;
    }
    if (uart->setFlowcontrol(_xonxoff, _rtscts) != mraa::SUCCESS) {
        std::cerr << "Error setting flow control UART" << std::endl;
    }
}
uart_module::~uart_module(){
    delete uart;
    delete temp;
}

void uart_module::close_uart(){
    uart->close();
}

void uart_module::send_packet(char* packet,int length){
    // std::cout << length <<std::endl;
    uart->write(packet,length);
}

int uart_module::recv_packet(char *packet,int max_size){
    char *temp = packet;
    int length = 0;
    int wait_count = 0;
    clock_t start, finish;
    start = clock();
    // while (wait_count < 1000) {
    //     if (uart->dataAvailable(0) == false) {
    //         // std::this_thread::sleep_for(std::chrono::microseconds(100));
    //         wait_count++;
    //     } else {
            while(uart->dataAvailable(0) && length < max_size) {//
                uart->read(temp, 1);
                temp++;
                length++;
            }
            finish=clock();
            // std::cout<<(finish-start)<<std::endl;
            // std::cout<<finish<<std::endl;
            // std::cout<<CLOCKS_PER_SEC<<std::endl;
            // std::this_thread::sleep_for(std::chrono::microseconds(100));
            // break;
    //     }
    // }
        return length;
}
