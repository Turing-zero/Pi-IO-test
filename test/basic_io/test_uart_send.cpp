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

/* UART port */
#define UART_PORT 0

const char *dev_path = "/dev/ttyAMA1";

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
    int send_delay = 1000;
    if (argc > 1) {
        baudrate = std::stoi(argv[1]);
        send_delay = std::stoi(argv[2]);
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
        gpio = new mraa::Gpio(40);
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

    int index = 0;
    int packet_count = 0;
    while (flag) {
        /* send data through uart */
        gpio->write(1);
        if(index & 0xff==0xff)packet_count++;
        int max_bytes = 10;
        char tx_buf[5] = {(char)0xff};
        tx_buf[0] = 0xab;
        tx_buf[1] = index & 0xff;
        tx_buf[2] = (index >> 8) & 0xff;
        tx_buf[3] = (index >> 8) & 0xff;
        tx_buf[4] = index & 0xff;
        tx_buf[5] = 0x00;
        tx_buf[6] = index & 0xff;
        // for(int i=5;i<max_bytes;i++)tx_buf[i]= 0x00;
        index++;

        uart->write(tx_buf,7);
        std::this_thread::sleep_for(std::chrono::microseconds(send_delay));
        // gpio->write(0);
    }
    //! [Interesting]

    delete uart;
    delete temp;
    delete gpio;

    return EXIT_SUCCESS;
}
