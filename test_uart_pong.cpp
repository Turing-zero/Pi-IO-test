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
#include <map>
#include <signal.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <unistd.h>

/* mraa headers */
#include "mraa/common.hpp"
#include "mraa/uart.hpp"
#include "mraa/gpio.hpp"

/* UART port */
#define UART_PORT 0
#define GPIO_PORT 40

const char *dev_path = "/dev/ttyAMA4";

volatile sig_atomic_t flag = 1;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Exiting..." << std::endl;
        flag = 0;
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    int baudrate = 115200;
    if (argc > 1) {
        baudrate = std::stoi(argv[1]);
        std::cout << "baudrate: " << baudrate << std::endl;
    }

    //! [Interesting]
    // If you have a valid platform configuration use numbers to represent uart
    // device. If not use raw mode where std::string is taken as a constructor
    // parameter
    mraa::Uart *uart, *temp;
    mraa::Gpio *gpio;

    try {
        uart = new mraa::Uart(dev_path);
        gpio = new mraa::Gpio(GPIO_PORT);
        gpio->dir(mraa::DIR_OUT);
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

    struct COMM_STATUS {
        int rx_count = 0;
        int tx_count = 0;

        int rx_index_start = 0;
        int rx_index_end = 0;
        int success_rate = 0;
    } comm_status;

    while (flag) {
        /* send data through uart */
        // uart->writeStr("Hello Mraa!\n");
        char s[256] = "";
        char *s_ptr = s;
        int len = 0;
        gpio->write(0);
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        while (uart->dataAvailable(0)) {
            uart->read(s_ptr, 1);
            s_ptr++;
            len++;
            // std::cout << "data available: " << int(s_ptr[0]) << std::endl;
            // std::cout << len << std::endl;
        }
        // if(s!=s_ptr)
        //     std::cout << "str: " << int(s[0]) << std::endl;
        if (s != s_ptr && s[0] == 0xab) {
            gpio->write(1);
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            char tx_buf[3] = {0xff};
            tx_buf[0] = 0xbc;
            tx_buf[1] = s[1];
            tx_buf[2] = s[2];

            uart->writeStr(tx_buf);
            if (comm_status.rx_count == 0) {
                comm_status.rx_index_start = s[1] | (s[2] << 8);
            } else {
                comm_status.rx_index_end = s[1] | (s[2] << 8);
            }
            comm_status.rx_count++;
            int rx_real_count = comm_status.rx_index_end - comm_status.rx_index_start + 1;
            comm_status.success_rate = comm_status.rx_count * 100 / rx_real_count;
            if (comm_status.rx_count % 100 == 0) {
                std::cout << "uart hit rate: " << (comm_status.success_rate) << std::endl;
            }
            std::cout << "recv str: " << int(s[0]) << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(10-2));
    }
    //! [Interesting]

    delete uart;
    delete temp;
    delete gpio;

    return EXIT_SUCCESS;
}
