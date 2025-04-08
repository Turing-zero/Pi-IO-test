/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributors: Manivannan Sadhasivam <manivannan.sadhasivam@linaro.org>
 * Copyright (c) 2015 Intel Corporation.
 *
 * SPDX-License-Identifier: MIT
 *
 * Example usage: Prints "Hello Mraa!" recursively. Press Ctrl+C to exit
 *
 */

/* standard headers */
#include <map>
#include "lib_io.h"
#include "utils.h"

#define UART_BUF_SIZE 127

int main(int argc, char **argv) {
    init_signal_handler();

    int baudrate = 115200;
    if (argc > 1) {
        baudrate = std::stoi(argv[1]);
        std::cout << "baudrate: " << baudrate << std::endl;
    }
    uart_module uart(baudrate,UART3);
    
    uart.open_uart();
    //! [Interesting]
    // If you have a valid platform configuration use numbers to represent uart
    // device. If not use raw mode where std::string is taken as a constructor
    // parameter

    struct COMM_STATUS {
        int rx_count = 0;
        int tx_count = 0;

        int rx_index_start = 0;
        int rx_index_end = 0;
        int success_rate = 0;
    } comm_status;

    while (g_signal_flag) {
        /* send data through uart */
        // uart.writeStr("Hello Mraa!\n");
        char s[UART_BUF_SIZE] = {0xff};
        uart.recv_packet(s, UART_BUF_SIZE, 200);
        // rs485->recv_485packet(s,15);
        if (s[0] == 0xab) {
            // std::cout << "data: ";
            // for (int i = 0; i < UART_BUF_SIZE; ++i) {
            //     std::cout << int(s[i]) << " ";
            // }
            // std::cout << std::endl;
            char tx_buf[UART_BUF_SIZE] = {0xff};
            tx_buf[0] = 0xbc;
            tx_buf[1] = s[1];
            tx_buf[2] = s[2];
            for (int i = 3; i < UART_BUF_SIZE; ++i) {
                tx_buf[i] = s[i];
            }

            uart.send_packet(tx_buf, UART_BUF_SIZE);
            // rs485->send_485packet(tx_buf);
            int rx_index = s[1] | (s[2] << 8);
            if (comm_status.rx_count == 0) {
                comm_status.rx_index_start = rx_index;
            } else{
                comm_status.rx_index_end = rx_index;
            }
            comm_status.rx_count++;
            int rx_real_count = comm_status.rx_index_end - comm_status.rx_index_start + 1;
            comm_status.success_rate = comm_status.rx_count * 100 / rx_real_count;
            if (comm_status.rx_count % 100 == 0) {
                std::cout << "uart hit rate: " << (comm_status.success_rate) << std::endl;
            }
            // std::cout << "recv str[1]: " << int(s[1]) << std::endl;
            // std::cout << "recv str[2]: " << int(s[2]) << std::endl;
        }

        // std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
    //! [Interesting]

    return EXIT_SUCCESS;
}
