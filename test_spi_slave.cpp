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
#include "mraa/spi.hpp"

/* UART port */
#define UART_PORT 0

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

    // int baudrate = 115200;
    int send_delay = 10;
    if (argc > 1) {
        // baudrate = std::stoi(argv[1]);
        // send_delay = std::stoi(argv[2]);
        // std::cout << "baudrate: " << baudrate << std::endl;
    }

    //! [Interesting]
    // If you have a valid platform configuration use numbers to represent uart
    // device. If not use raw mode where std::string is taken as a constructor
    // parameter
    // mraa::Spi *spi1;
    mraa::Gpio *MOSI,*MISO,*SCLK,*CS;
    try {
        // spi1 = new mraa::Spi(1);
        // MISO = new mraa::Gpio(21);
        // MISO->dir(mraa::DIR_IN);
        // MOSI = new mraa::Gpio(19);
        // MOSI->dir(mraa::DIR_OUT);
        // SCLK = new mraa::Gpio(23);
        // SCLK->dir(mraa::DIR_OUT);
        // CS = new mraa::Gpio(24);
        // CS->dir(mraa::DIR_OUT);
    } catch (std::exception &e) {
        std::cerr << "Error while setting up raw SPI, do you have a spi?" << std::endl;
        std::terminate();
    }
    // if (spi1->mode(mraa::Spi_Mode::SPI_MODE0) != mraa::SUCCESS) {
    //     std::cerr << "Error setting parity on SPI0" << std::endl;
    // }
    // if (spi1->frequency(500000) != mraa::SUCCESS) {
    //     std::cerr << "Error setting parity on SPI0" << std::endl;
    // }


    int index = 0;
    int bufidx = 0;
    while (flag) {
        /* send data through uart */
        uint8_t tx_buf[7] = {0xff};
        uint8_t rx_buf[7];
        tx_buf[0] = 0xab;
        tx_buf[1] = index & 0xff;
        tx_buf[2] = (index >> 8) & 0xff;

        while(!CS->read()){//片选信号有效
            while(!SCLK->read());//等待时钟上升沿
            int bit=MOSI->read();
            rx_buf[bufidx/8]<<=1;
            rx_buf[bufidx/8] |=bit;
            bufidx++;
            while(SCLK->read());//等待时钟下降沿
            std::cout << "Received data: ";
            // for (int i = 0; i < BUFFER_SIZE; ++i) {
                std::cout << "0x" << std::hex << static_cast<int>(rx_buf[0]) << " ";
            // }
            std::cout << std::endl;
        }

        index++;

        std::this_thread::sleep_for(std::chrono::microseconds(send_delay));
    }
    //! [Interesting]

    delete /*spi0,*/MISO,MOSI,CS;

    return EXIT_SUCCESS;
}
