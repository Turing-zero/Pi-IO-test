#include "lib_io.h"

int main() {
    uart_module uart(115200, UART2);
    uart.open_uart();
    char packet[50];
    packet[0] = 0xff;
    packet[1] = 0x0a;
    for (int i = 0; i < 50; i++) {
        packet[2] = i;
        uart.send_packet(packet, 50);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    uart.close_uart();
    return 0;
}