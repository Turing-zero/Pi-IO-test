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
#include <time.h>

/* mraa headers */
#include "mraa/common.hpp"
#include "mraa/gpio.hpp"
#include "mraa/spi.hpp"

/* ADC CHANNEL  */
#define NUM_CHANNEL 1
const double ref_v = 4.096;

const int BUFFER_SIZE = 128;

volatile sig_atomic_t flag = 1;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Exiting..." << std::endl;
        flag = 0;
    }
}

double readADC(mraa::Spi *spi,int channel,double ref_v){
     /* read ADC */
    uint8_t send_data[BUFFER_SIZE] = {0x00,0x00};
    uint8_t recv_data[BUFFER_SIZE] = {0};
    uint16_t config = 0xF120;
    config |= channel << 9;
    send_data[0] = config >> 8 & 0xFF;
    send_data[1] = config & 0xFF;
    spi->transfer(send_data,recv_data,2);
    // spi0->write(send_data,2);
    uint16_t result = (recv_data[0]<<8) | recv_data[1];
    double voltage = (result/65535.0)*ref_v;
    return voltage;
} 

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    int send_delay = 1;
    if (argc > 1) {
    }

    //! [Interesting]
    // If you have a valid platform configuration use numbers to represent uart
    // device. If not use raw mode where std::string is taken as a constructor
    // parameter
    mraa::Spi *spi0;
    mraa::Gpio *MOSI,*MISO,*SCLK,*CE;
    try {
        spi0 = new mraa::Spi(0);
    } catch (std::exception &e) {
        std::cerr << "Error while setting up raw SPI, do you have a spi?" << std::endl;
        std::terminate();
    }
    if (spi0->mode(mraa::SPI_MODE0) != mraa::SUCCESS) {
        std::cerr << "Error setting mode on SPI0" << std::endl;
    }
    if (spi0->frequency(1000000) != mraa::SUCCESS) {
        std::cerr << "Error setting frequency on SPI0" << std::endl;
    }
    if (spi0->bitPerWord(8) != mraa::SUCCESS) {
        std::cerr << "Error setting bitPerWord on SPI0" << std::endl;
    }

    // 发送和接收的数据缓冲区

    // 填充发送数据
    // std::string message = "Hello from Raspberry Pi!";
    // std::memcpy(send_data, message.c_str(), message.size());
    int index = 0;
    double diff = 0;
    int count = 0;
    int cnt=0;
    int total = 0;
    int last_v = 0;
    double adc_value[NUM_CHANNEL];
    struct timespec start,end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (flag) {
        // diff +=abs(voltage-1.0); 
        // if(last_v==voltage){
        //     count++;
        // }else{
        //     if(count>total){
        //         total = count;
        //     }
        //     count = 0;
        // }
        cnt++;

        clock_gettime(CLOCK_MONOTONIC, &end);
        // std::cout<<end.tv_sec<<std::endl;
        double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;
        if (elapsed >= 1.0) {  // 统计1秒内的采样次数
            std::cout<<"samples per second:"<<cnt<<std::endl;
            cnt=0;
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
        // if(cnt==10000){
        //     std::cout << diff/10000.0 << std::endl;
        //     count=0;
        //     diff=0;
        //     cnt=0;
        //     std::cout<<total<<std::endl;
        //     // printf("当前时间：%02d:%02d:%02d.%06ld   voltage:%f \n", tm->tm_hour, tm->tm_min, tm->tm_sec, ts.tv_nsec/1000,voltage);
        
        // } 
        for(int i=0;i<NUM_CHANNEL;i++){
            adc_value[i]=readADC(spi0,i,ref_v);
        }
        for(int i=0;i<NUM_CHANNEL;i++){
            std::cout<<"ADC Channel"<<i<<":"<<adc_value[i]<<std::endl;
        }
        // std::cout << "ADC Voltage from IN"<<CHANNEL<<":" << voltage << " V" << std::endl;
       
        // 将接收到的数据转换为字符串
        // std::string recv_str(reinterpret_cast<char*>(recv_data), BUFFER_SIZE);
        // std::cout << "Received from ESP32-C6: " << recv_str << std::endl;
        index++;
        // last_v=voltage;

        // std::this_thread::sleep_for(std::chrono::microseconds(send_delay));
    }
    //! [Interesting]

    delete spi0,MISO,MOSI,CE;

    return EXIT_SUCCESS;
}
