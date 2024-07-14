#include "io.h"

mraa::Uart* uart_init(const char* device, struct uart_params params) {
    mraa::Uart* uart_handle;
    try {
        uart_handle = new mraa::Uart(device);
    } catch (std::exception& e) {
        std::cerr << "Error while setting up raw UART, do you have a uart?" << std::endl;
        std::terminate();
    }
    
    if (uart_handle->setBaudRate(params.baudrate) != mraa::SUCCESS) {
        std::cerr << "Error setting parity on UART" << std::endl;
    }
    if (uart_handle->setMode(params.data_bits, params.parity, params.stop_bits) != mraa::SUCCESS) {
        std::cerr << "Error setting parity on UART" << std::endl;
    }
    if (uart_handle->setFlowcontrol(params.xonxoff, params.rtscts) != mraa::SUCCESS) {
        std::cerr << "Error setting flow control UART" << std::endl;
    }
    return uart_handle;
}

int uart_read(mraa::Uart* uart_handle, char* rx_buf, int max_size) {
    int length = 0;
    // char s[UART_MAX_LENGTH] = "";
    char *s_ptr = rx_buf;
    while (uart_handle->dataAvailable(0) && length < max_size) {
        uart_handle->read(s_ptr, 1);
        s_ptr++;
        length++;
    }
    return length;
}