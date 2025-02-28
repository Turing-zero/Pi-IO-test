#include "lib_io.h"

int main(){
    uart_module uart(921600,UART2);
    uart.open_uart();
    char send_buf[2]={0x55,0x50};
    while(1){
        uart.send_packet(send_buf,2);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        char read_buf[256];

        int len = uart.recv_packet(read_buf,256);
        std::cout << len <<std::endl;
    }
}