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

const int BUFFER_SIZE = 128;

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
    int send_delay = 800;
    if (argc > 1) {
        // baudrate = std::stoi(argv[1]);
        // send_delay = std::stoi(argv[2]);
        // std::cout << "baudrate: " << baudrate << std::endl;
    }

    //! [Interesting]
    // If you have a valid platform configuration use numbers to represent uart
    // device. If not use raw mode where std::string is taken as a constructor
    // parameter
    mraa::Spi *spi0;
    mraa::Gpio *MOSI,*MISO,*SCLK,*CE;
    try {
        spi0 = new mraa::Spi(0);
        // MISO = new mraa::Gpio(21);
        // MISO->dir(mraa::DIR_IN);
        // MOSI = new mraa::Gpio(19);
        // MOSI->dir(mraa::DIR_OUT);
        // SCLK = new mraa::Gpio(23);
        // SCLK->dir(mraa::DIR_OUT);
        // CE = new mraa::Gpio(24);
        // CE->dir(mraa::DIR_OUT);
    } catch (std::exception &e) {
        std::cerr << "Error while setting up raw SPI, do you have a spi?" << std::endl;
        std::terminate();
    }
    if (spi0->mode(mraa::SPI_MODE0) != mraa::SUCCESS) {
        std::cerr << "Error setting parity on SPI0" << std::endl;
    }
    if (spi0->frequency(500000) != mraa::SUCCESS) {
        std::cerr << "Error setting parity on SPI0" << std::endl;
    }
    if (spi0->bitPerWord(8) != mraa::SUCCESS) {
        std::cerr << "Error setting parity on SPI0" << std::endl;
    }

    // 发送和接收的数据缓冲区
    uint8_t send_data[BUFFER_SIZE] = {0x01, 0x02, 0x03};;
    uint8_t recv_data[BUFFER_SIZE] = {0};

    // 填充发送数据
    std::string message = "Hello from Raspberry Pi!";
    std::memcpy(send_data, message.c_str(), message.size());

    int index = 0;
    while (flag) {
        /* send data through uart */
        // int max_bytes = 10;
        // uint8_t tx_buf[7] = {0xff};
        // uint8_t rx_buf[7];
        // tx_buf[0] = 0xab;
        // tx_buf[1] = index & 0xff;
        // tx_buf[2] = (index >> 8) & 0xff;
        // tx_buf[3] = (index >> 8) & 0xff;
        // tx_buf[4] = index & 0xff;
        // tx_buf[5] = 0x00;
        // tx_buf[6] = index & 0xff;
        // for(int i=5;i<max_bytes;i++)tx_buf[i]= 0x00;
        spi0->transfer(send_data,recv_data,BUFFER_SIZE);

        // 将接收到的数据转换为字符串
        std::string recv_str(reinterpret_cast<char*>(recv_data), BUFFER_SIZE);
        std::cout << "Received from ESP32-C6: " << recv_str << std::endl;
        index++;

        std::this_thread::sleep_for(std::chrono::microseconds(send_delay));
        // gpio->write(0);
    }
    //! [Interesting]

    delete spi0,MISO,MOSI,CE;

    return EXIT_SUCCESS;
}
