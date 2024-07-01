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
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <chrono>
#include <thread>

/* mraa headers */
#include "mraa/common.hpp"
#include "mraa/uart.hpp"

/* UART port */
#define UART_PORT 0

const char* dev_path = "/dev/ttyAMA0";

volatile sig_atomic_t flag = 1;

void
sig_handler(int signum)
{
    if (signum == SIGINT) {
        std::cout << "Exiting..." << std::endl;
        flag = 0;
    }
}

int
main(void)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // If you have a valid platform configuration use numbers to represent uart
    // device. If not use raw mode where std::string is taken as a constructor
    // parameter
    mraa::Uart* uart, *temp;
    try {
        uart = new mraa::Uart(dev_path);
    } catch (std::exception& e) {
        std::cerr << "Error while setting up raw UART, do you have a uart?" << std::endl;
        std::terminate();
    }
    if (uart->setBaudRate(4000000) != mraa::SUCCESS) {
        std::cerr << "Error setting parity on UART" << std::endl;
    }
    if (uart->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
        std::cerr << "Error setting parity on UART" << std::endl;
    }
    if (uart->setFlowcontrol(false, false) != mraa::SUCCESS) {
        std::cerr << "Error setting flow control UART" << std::endl;
    }

    int index = 0;
    while (flag) {
        /* send data through uart */
        char tx_buf[5] = {0xff};
        tx_buf[0] = 0xab;
        tx_buf[1] = index & 0xff;
        tx_buf[2] = (index >> 8) & 0xff;
        index++;
        uart->writeStr(tx_buf);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    //! [Interesting]

    delete uart;
    delete temp;

    return EXIT_SUCCESS;
}
