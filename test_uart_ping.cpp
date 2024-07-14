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
#include <chrono>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <unistd.h>

/* mraa headers */
#include "mraa/common.hpp"
#include "mraa/uart.hpp"
#include "mraa/gpio.hpp"

#include "io.h"

/* UART port */
#define UART_PORT 0
#define UART_PACK_LEN 25

/*UART DELAY*/
// #define UART_DELAY 1 //us

const char *dev_path = "/dev/ttyAMA2";

volatile sig_atomic_t flag = 1;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Exiting..." << std::endl;
        flag = 0;
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    struct uart_params _p;
    // uart param
    int send_delay = 10;
    int UART_DELAY = 400;
    if (argc > 1) {
        _p.baudrate = std::stoi(argv[1]);
        send_delay = std::stoi(argv[2]);
        std::cout << "baudrate: " << _p.baudrate << std::endl;
    }

    // init uart
    mraa::Uart *uart = uart_init(dev_path, _p);

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
            pingpong_delay_ave = pingpong_delay_sum / pingpong_delay_count;
            std::cout << "pingpong delay: " << pingpong_delay_ave << "ms" << std::endl;
            std::cout << "test for 1min: finished." << std::endl;
            break;
        }

        // std::cout << timestamp_ping << std::endl;
        /* send data through uart */
        char tx_buf[UART_PACK_LEN] = {0xff};
        tx_buf[0] = 0xab;
        tx_buf[1] = index & 0xff;
        tx_buf[2] = 0xff;//(index >> 8) & 0xff;
        for(int i=3;i<15;++i)tx_buf[i]=0xA5;

        // tx_buf[3] = timestamp_ping & 0xff;
        // tx_buf[4] = (timestamp_ping >> 8) & 0xff;
        // tx_buf[5] = (timestamp_ping >> 16) & 0xff;
        // tx_buf[6] = (timestamp_ping >> 24) & 0xff;
        uart->writeStr(tx_buf);
        std::this_thread::sleep_for(std::chrono::microseconds(UART_DELAY));
        gpio->write(0);
        // std::this_thread::sleep_for(std::chrono::microseconds(1));
        // Pong
        char s[UART_PACK_LEN] = "";
        int wait_count = 0;
        int recv_len = 0;
        while (wait_count < 1000) {
            if (uart->dataAvailable(0) == false) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
                wait_count++;
            } else {
                recv_len = uart_read(uart, s, UART_PACK_LEN);
                // std::cout << "data available: " << (int)(s[0]) << " " << (int)(s[1]) << " " << (int)(s[2]) << " " << index << std::endl;
                break;
            }
        }
        if (wait_count == 10000) {
            index = 0;
        } 

        // Check ping-pong
        if (s[0] == 0xbc) {
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

    delete uart;

    return EXIT_SUCCESS;
}
