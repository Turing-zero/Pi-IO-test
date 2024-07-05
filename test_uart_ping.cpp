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

/* UART port */
#define UART_PORT 0

const char *dev_path = "/dev/ttyAMA0";

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
    if (argc > 1) {
        baudrate = std::stoi(argv[1]);
        send_delay = std::stoi(argv[2]);
        std::cout << "baudrate: " << baudrate << std::endl;
    }

    // init uart
    mraa::Uart *uart, *temp;
    try {
        uart = new mraa::Uart(dev_path);
    } catch (std::exception &e) {
        std::cerr << "Error while setting up raw UART, do you have a uart?" << std::endl;
        std::terminate();
    }
    if (uart->setBaudRate(baudrate) != mraa::SUCCESS) {
        std::cerr << "Error setting parity on UART" << std::endl;
    }
    if (uart->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
        std::cerr << "Error setting parity on UART" << std::endl;
    }
    if (uart->setFlowcontrol(false, false) != mraa::SUCCESS) {
        std::cerr << "Error setting flow control UART" << std::endl;
    }

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

        // std::cout << timestamp_ping << std::endl;
        /* send data through uart */
        char tx_buf[25] = {0xff};
        tx_buf[0] = 0xab;
        tx_buf[1] = index & 0xff;
        tx_buf[2] = (index >> 8) & 0xff;

        // tx_buf[3] = timestamp_ping & 0xff;
        // tx_buf[4] = (timestamp_ping >> 8) & 0xff;
        // tx_buf[5] = (timestamp_ping >> 16) & 0xff;
        // tx_buf[6] = (timestamp_ping >> 24) & 0xff;
        uart->writeStr(tx_buf);

        // Pong
        char s[25] = "";
        char *s_ptr = s;
        int wait_count = 0;
        while (wait_count < 1000) {
            if (uart->dataAvailable(0) == false) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
                wait_count++;
            } else {
                while (uart->dataAvailable(0)) {
                    uart->read(s_ptr, 1);
                    s_ptr++;
                }
                break;
            }
        }
        if (wait_count == 10000) {
            index = 0;
        }

        // Check ping-pong
        if (s != s_ptr && s[0] == 0xbc) {
            // std::cout << "data available: " << (int)(s[0]) << " " << (int)(s[1]) << " " << (int)(s[2]) << " " << index << std::endl;
            int index_pong = s[1] | (s[2] << 8);
            ;
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
        }

        index++;
        std::this_thread::sleep_for(std::chrono::milliseconds(send_delay));
    }
    //! [Interesting]

    delete uart;
    delete temp;

    return EXIT_SUCCESS;
}
