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
#include <signal.h>
#include "lib_io.h"

volatile sig_atomic_t flag = 1;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Exiting..." << std::endl;
        flag = 0;
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    // uart param
    int baudrate = 115200;
    int send_delay = 10;
    int UART_DELAY = 400;
    if (argc > 1) {
        baudrate = std::stoi(argv[1]);
        send_delay = std::stoi(argv[2]);
        UART_DELAY = std::stoi(argv[3]);
        std::cout << "baudrate: " << baudrate << std::endl;
        std::cout << "send_delay: " << send_delay << std::endl;
        std::cout << "uart_delay: " << UART_DELAY << std::endl;
    }
    uart_module uart(baudrate,UART3);

    //open serial
    // rs485->open_rs485();
    uart.open_uart();
    int index = 0;
    float pingpong_delay_sum = 0; // ms
    float pingpong_delay_ave = 0; // ms
    int pingpong_delay_count = 0;
    int64_t timestamp_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    while (flag) {
        // Ping
        int64_t timestamp_ping = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (timestamp_ping - timestamp_start > 1000000 * 60) {
            flag = false;
            std::cout << "test for 1min: finished." << std::endl;
        }
        char tx_buf[25] = {(char)0xff};
        tx_buf[0] = 0xab;
        tx_buf[1] = index & 0xff;
        tx_buf[2] = 0xff;//(index >> 8) & 0xff;
        for(int i=3;i<15;++i)tx_buf[i]=0xA5;


        uart.send_packet(tx_buf,15);
        // Pong
        char s[256] = "";
        uart.recv_packet(s,15);

        // Check ping-pong
        if ( s[0] == 0xbc) {
            // std::cout << "data available: " << (int)(s[0]) << " " << (int)(s[1]) << " " << (int)(s[2]) << " " << index << std::endl;
            std::cout << "data: ";
            for(int i=0;i<15;++i)std::cout <<int(s[i])<<" ";
            std::cout << std::endl;
            int index_pong = s[1] | (s[2] << 8);
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
            // std::cout << "recv str: " << int(s[0]) << std::endl;
            // std::cout << "index: " << int(index) << std::endl;
            // std::cout << "indexpong: " << int(index_pong) << std::endl;
        }

        index++;
        std::this_thread::sleep_for(std::chrono::milliseconds(send_delay));
    }
    //! [Interesting]

    return EXIT_SUCCESS;
}
