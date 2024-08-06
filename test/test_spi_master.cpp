#include <chrono>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include <cstring>
#include "rpi_lib.h"
#include <time.h>

const int BUFFER_SIZE = 5;

volatile sig_atomic_t flag = 1;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Exiting..." << std::endl;
        flag = 0;
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    int send_delay = 0;
    if (argc > 1) {
    }

    spi_module spi(0,0,3,500000,8,false);
    spi.open_spi();
    int index = 0;
    struct timespec start,end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int cnt=0;
    while (flag) {
        cnt++;
        // 发送和接收的数据缓冲区
        uint8_t send_data[BUFFER_SIZE] = {0x01, 0x02, 0x03};;
        uint8_t recv_data[BUFFER_SIZE] = {0};
        send_data[0] = 0xab;
        send_data[1] = 0x01;
        send_data[2] = 0x02;
        send_data[3] = 0x03;
        send_data[4] = 0x04;
        for(int i=5;i<BUFFER_SIZE;++i){
            send_data[i]=i;
        }
        spi.transfer(send_data,recv_data,BUFFER_SIZE);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;
        if (elapsed >= 1.0) {  // 统计1秒内的采样次数
            std::cout<<"samples per second:"<<cnt<<std::endl;
            cnt=0;
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
        // 将接收到的数据转换为字符串
        std::cout << "Received from ESP32-C6: ";
        for(int i=0;i<BUFFER_SIZE;++i){
            std::cout << (int)recv_data[i]<< " "; 
        }
        std::cout<<std::endl;
        index++;
    }

    spi.close_spi();

    return EXIT_SUCCESS;
}
