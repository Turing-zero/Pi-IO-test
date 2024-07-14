#include <chrono>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <unistd.h>

/* mraa headers */
#include "mraa/common.hpp"
#include "mraa/uart.hpp"

#define UART_MAX_LENGTH 256

struct uart_params {
    int baudrate = 115200;
    int data_bits = 8;
    mraa::UartParity parity = mraa::UART_PARITY_NONE;
    int stop_bits = 1;

    bool xonxoff = false;
    bool rtscts = false;
};

mraa::Uart* uart_init(const char* device, struct uart_params params);

int uart_read(mraa::Uart* uart_handle, char* rx_buf, int max_size=UART_MAX_LENGTH);