#include <chrono>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <time.h>

/* mraa headers */
#include "rpi_lib.h"

/* ADC CHANNEL  */
#define NUM_CHANNEL 1
const double ref_v = 5;

const int BUFFER_SIZE = 128;

volatile sig_atomic_t flag = 1;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        std::cout << "Exiting..." << std::endl;
        flag = 0;
    }
}

double readADC(spi_module &spi,int channel,double ref_v){
     /* read ADC */
    uint8_t send_data[BUFFER_SIZE] = {0x00,0x00};
    uint8_t recv_data[BUFFER_SIZE] = {0};
    uint16_t config = 0xF1C0;
    config |= channel << 9;
    send_data[0] = config >> 8 & 0xFF;
    send_data[1] = config & 0xFF;
    spi.transfer(send_data,recv_data,2);
    uint16_t result = (recv_data[0]<<8) | recv_data[1];
    double voltage = (result/65535.0)*ref_v;
    return voltage;
} 

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);

    int send_delay = 1;
    if (argc > 1) {
    }

    spi_module spi(0,0,0,200000,8,false);
    spi.open_spi();

    // 发送和接收的数据缓冲区

    // 填充发送数据
    int index = 0;
    double diff = 0;
    int count = 0;
    int cnt=0;
    int last_v = 0;
    double adc_value[NUM_CHANNEL];
    struct timespec start,end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    double total = 0;
    while (flag){
        cnt++;
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;
        if (elapsed >= 1.0) {  // 统计1秒内的采样次数
            std::cout<<"samples per second:"<<cnt<<std::endl;
            cnt=0;
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
        //多通道ADC测试
        // for(int i=0;i<NUM_CHANNEL;i++){
        //     adc_value[i]=readADC(spi,i,ref_v);
        // }
        // for(int i=0;i<NUM_CHANNEL;i++){
        //     std::cout<<"ADC Channel"<<i<<":"<<adc_value[i]<<std::endl;
        // }
        double voltage = readADC(spi,0,ref_v);
        total+=abs(voltage-0);
        count++;
        if(count==1000){
            std::cout<< total/1000.0 <<std::endl;
            count=0;
            total = 0;
        }
        // std::cout<<"ADC Channel0"<<":"<<readADC(spi,0,ref_v)<<std::endl;
       

        index++;

    }

    return EXIT_SUCCESS;
}
