#ifndef __RPI_LIB__
#define __RPI_LIB__
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mraa/common.hpp"
#include "mraa/uart.hpp"
#include "mraa/gpio.hpp"
#include "mraa/spi.hpp"
#include "mraa/i2c.hpp"


#define MAX_UDP_BUFFER_SIZE 1024
enum Uart_Port{
    UART0=0,
    UART1,
    UART2,
    UART3,
    UART4,
    UART5,
};

enum Channel{
    vr1 = 0,
    vr2 = 1,
    sw0 = 2,
    sw1 = 3,
    sw2 = 4,
    sw3 = 5,
    sw4 = 6
};
namespace devices{
    inline  std::string dev_path[6]={
        "/dev/ttyAMA0",
        "/dev/ttyS0",
        "/dev/ttyAMA1",
        "/dev/ttyAMA2",
        "/dev/ttyAMA3",
        "/dev/ttyAMA4",
    };
    
};
class uart_module{
    public:
        uart_module(int baudrate = 115200,Uart_Port port=UART0,int databyte=8,mraa::UartParity parity=mraa::UART_PARITY_NONE,int stopbits = 1,bool xonxoff=false,bool rtscts=false);
        ~uart_module();
        void setBaudRate(int baudrate){ _baudrate = baudrate;}
        void setUartPort(Uart_Port port){ _port=port;}
        void setDataByte(int databyte){ _databyte=databyte;}
        void setParity(mraa::UartParity parity){ _parity=parity;}
        void setStopBits(int stopbits){ _stopbyte=stopbits;}
        void setXonxoff(bool xonxoff){ _xonxoff=xonxoff;}
        void setRtscts(bool rtscts){ _rtscts=rtscts;}
        int getBaudRate(){return _baudrate;}
        Uart_Port getUartPort(){return _port;}
        int getDataByte(){ return _databyte;}
        mraa::UartParity getParity(){ return _parity;}
        int getStopBits(){ return _stopbyte;}
        bool getXonxoff(){ return _xonxoff;}
        bool getRtscts(){ return _rtscts;}
        void open_uart();
        void close_uart();
        void send_packet(char* packet,int length);
        int recv_packet(char *packet,int max_size);
        static unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);
    private:
        int _baudrate;
        Uart_Port _port;
        int _databyte;
        int _stopbyte;
        bool _xonxoff;
        bool _rtscts;
        mraa::UartParity _parity;
        mraa::Uart *uart=NULL, *temp=NULL;
};

class rs485_module:public uart_module{
    public:
        rs485_module(int baudrate = 115200,int uart_delay=1200 ,Uart_Port port=UART0,int sw_gpio=-1,int databyte=8,mraa::UartParity parity=mraa::UART_PARITY_NONE,int stopbits = 1,bool xonxoff=false,bool rtscts=false);
        ~rs485_module();
        void open_rs485();
        void close_rs485();
        void setUartDelay(int uart_delay){ _uart_delay=uart_delay;}
        void set_RsSwPort(int port){ _Rs_SwPort=port;}
        int get_RsSwPort(){return _Rs_SwPort;}
        int getUartDelay(){return _uart_delay;}
        void send_485packet(char* packet,int length);
        int recv_485packet(char *packet,int max_size);
    private:
        int _uart_delay;
        int _Rs_SwPort;
        mraa::Gpio *gpio;
};

class spi_module{
    public:
        spi_module(int bus=0,int cs=0,int spimode=0,int frequency=1000000,int bitPerWord=8,bool lsbmode=false);
        ~spi_module(); 
        void open_spi();
        void close_spi();
        mraa::Spi_Mode get_mode(int spimode);  
        void transfer(uint8_t* tx_buf,uint8_t* rx_buf,int length);
        void transfer_word(uint16_t* tx_buf,uint16_t* rx_buf,int length);
    private:
        int _bus;
        int _cs;
        int _spimode;
        int _frequency;
        int _bitPerWord;
        bool _lsbmode;//fasle为从高到低收发，反之从低到高
        mraa::Spi *spi;
};

class udp_module{
    public:
        udp_module(char* ip,int port);
        void open_udp();
        void close_udp();
        void send_pkg(char *message);
        int recv_pkg(char *recv_msg);
    private:
        int _port;
        int _sockfd;
        char* _ip;
        char _recvmsg[MAX_UDP_BUFFER_SIZE];
        int _buffer_size;
        sockaddr_in server_addr, client_addr;
        socklen_t client_addr_len;
};

class i2c_module{
    public:
        i2c_module(int bus);
        ~i2c_module();
        void open_i2c();
        mraa::Result write(uint8_t addr,uint8_t*data,int length);
        int read(uint8_t addr,uint8_t*data,int length);
        mraa::Result writeByte(uint8_t addr,uint8_t data);
        uint8_t readByte(uint8_t addr,uint8_t data);
        int readBytesReg(uint8_t addr,uint8_t reg,uint8_t *data,int length);
        void close_i2c();
    private:
        int _bus;
        mraa::I2c *i2c;
};
#endif