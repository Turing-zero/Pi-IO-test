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
#include <cstring>
#include "rpi_lib.h"
#include <time.h>

const int BUFFER_SIZE = 25;

volatile sig_atomic_t flag = 1;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Exiting..." << std::endl;
        flag = 0;
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    int64_t timestamp_ping = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();    

    int send_delay = 1800;
    if (argc > 1) {
    }

    //! [Interesting]
    // If you have a valid platform configuration use numbers to represent uart
    // device. If not use raw mode where std::string is taken as a constructor
    // parameter
    


    // 填充发送数据
    // std::string message = "Hello from Raspberry Pi!";
    // std::memcpy(send_data, message.c_str(), message.size());

    spi_module spi(0,0,0,8000000,8,false);
    spi.open_spi();
    int index = 0;
    struct timespec start,end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int cnt=0;
    float pingpong_delay_sum = 0; // ms
    float pingpong_delay_ave = 0; // ms
    int pingpong_delay_count = 0;
    while (flag) {
        cnt++;
        // 发送和接收的数据缓冲区
        uint8_t send_data[BUFFER_SIZE] = {0x01, 0x02, 0x03};;
        uint8_t recv_data[BUFFER_SIZE] = {0};
        send_data[0] = 0xab;
        send_data[1] = index & 0x0f;
        send_data[2] = index & 0xf0;
        send_data[3] = (index >> 8) & 0x0f;
        send_data[4] = (index >> 8) & 0xf0;
        // tx_buf[5] = 0x00;
        // tx_buf[6] = index & 0xff;
        spi.transfer(send_data,recv_data,BUFFER_SIZE);
        clock_gettime(CLOCK_MONOTONIC, &end);
        // std::cout<<end.tv_sec<<std::endl;
        // double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;
        // if (elapsed >= 1.0) {  // 统计1秒内的采样次数
        //     std::cout<<"samples per second:"<<cnt<<std::endl;
        //     cnt=0;
        //     clock_gettime(CLOCK_MONOTONIC, &start);
        // }
        // std::cout << (int)recv_data[0] <<std::endl;
        if(recv_data[0]==0x10){
            int index_pong = recv_data[1] | (recv_data[2] << 4) | (recv_data[3] << 8) | (recv_data[4] << 12);
            // std::cout<<recv_data[1]<<std::endl;
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
        // 将接收到的数据转换为字符串
        // std::string recv_str(reinterpret_cast<char*>(recv_data), BUFFER_SIZE);
        // std::cout << "Received from ESP32-C6: " << recv_str << std::endl;
        index++;

        std::this_thread::sleep_for(std::chrono::microseconds(send_delay));
    }
    //! [Interesting]

    spi.close_spi();

    return EXIT_SUCCESS;
}
