#include "lib_io.h"

#define I2C_SLAVE_ADDR 0x3C // 替换为你的I2C从设备地址
#define BUFFER_SIZE 25

int main() {
    // 初始化I2C设备
    mraa::I2c *i2c; // 使用I2C bus 1

    try {
        i2c = new mraa::I2c(0);
    } catch (std::exception &e) {
        std::cerr << "Error while setting up raw i2c, do you have a i2c" << 1 <<"?" <<std::endl;
        std::terminate();
    }
    if (i2c->address(I2C_SLAVE_ADDR) != mraa::SUCCESS) {
        std::cerr << "Error setting address on I2c" <<I2C_SLAVE_ADDR<< std::endl;
    }

    struct timespec start,end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int cnt = 0;
    while(1){
        // 写入数据到从设备
        uint8_t data[BUFFER_SIZE];
        for(int i=0;i<BUFFER_SIZE;++i){
            data[i] = i;
        }

        mraa::Result result = i2c->write(data,BUFFER_SIZE);

        // 从从设备读取数据
        uint8_t data_read[BUFFER_SIZE];
        i2c->read(data_read,BUFFER_SIZE);

        cnt++;
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1e9;
        if (elapsed >= 1.0) {  // 统计1秒内的采样次数
            std::cout<<"samples per second:"<<cnt<<std::endl;
            cnt=0;
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
    }

    return 0;
}