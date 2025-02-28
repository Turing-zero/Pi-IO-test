#include "lib_io.h"
#include "utils.h"

int main() {
    init_signal_handler();

    uart_module uart(115200, UART3);
    uart.open_uart();
    uint8_t packet[50];
    while (g_signal_flag) {
        int length = uart.recv_packet((char*)packet, 50, 50);
        if (length > 0) {
            printf("%x\n", (packet[2]));
        }else {
            uart.clear_rxbuf();
        }
    }
    uart.close_uart();
    return 0;
}