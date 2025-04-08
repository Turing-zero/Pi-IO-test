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
#include "lib_io.h"
#include "utils.h"

#define UART_BUF_SIZE 127

int main(int argc, char **argv) {
    init_signal_handler();

    // uart param
    int baudrate = 115200;
    int send_delay = 10;
    if (argc > 1) {
        baudrate = std::stoi(argv[1]);
        send_delay = std::stoi(argv[2]);
        std::cout << "baudrate: " << baudrate << std::endl;
        std::cout << "send_delay: " << send_delay << std::endl;
    }
    uart_module uart(baudrate, UART2);

    //open serial
    // rs485->open_rs485();
    uart.open_uart();
    int index = 0;
    float pingpong_delay_sum = 0; // ms
    float pingpong_delay_ave = 0; // ms
    int pingpong_delay_count = 0;
    int64_t timestamp_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    while (g_signal_flag) {
        int64_t timestamp_ping = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); // us
        if (timestamp_ping - timestamp_start > 1e6 * 30) { // 30 seconds
            std::cout << "test for 1min: finished." << std::endl;
            break;
        }

        char tx_buf[UART_BUF_SIZE] = {0xff};
        tx_buf[0] = 0xab;
        tx_buf[1] = index & 0xff;
        tx_buf[2] = (index >> 8) & 0xff;
        // for (int i = 3; i < UART_BUF_SIZE; ++i) {
        //     tx_buf[i] = 0xA5;
        // }

        // Ping
        uart.send_packet(tx_buf, UART_BUF_SIZE);
        // Pong
        char rx_buf[UART_BUF_SIZE] = {0};
        uart.recv_packet(rx_buf, UART_BUF_SIZE, 100);

        // Check ping-pong
        if ( rx_buf[0] == 0xbc) {
            {
                // Debug
                // std::cout << "data: ";
                // for (int i = 0; i < UART_BUF_SIZE; ++i) {
                //     std::cout << int(rx_buf[i]) << " ";
                // }
                // std::cout << std::endl;
            }
            int index_pong = rx_buf[1] | (rx_buf[2] << 8);
            if (index == index_pong) {
                int64_t timestamp_pong = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

                int pingpong_delay = (timestamp_pong - timestamp_ping) / 1000.0;
                pingpong_delay_sum += pingpong_delay;
                pingpong_delay_count++;
                // Output
                if (pingpong_delay_count % 100 == 0) {
                    pingpong_delay_ave = pingpong_delay_sum / pingpong_delay_count;
                    std::cout << "pingpong delay: " << pingpong_delay_ave << "ms" << std::endl;
                }
            }
            // std::cout << "recv str: " << int(rx_buf[0]);
            // std::cout << "; index: " << int(index);
            // std::cout << "; indexpong: " << int(index_pong) << std::endl;
        }

        index++;
        std::this_thread::sleep_for(std::chrono::milliseconds(send_delay));
    }
    //! [Interesting]

    return EXIT_SUCCESS;
}
