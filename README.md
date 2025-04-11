# tbk-IO

## 安装依赖

测试平台为Raspberry Pi 4 Model B，Ubuntu 22.04

1. compile tools
```bash
sudo apt install build-essential cmake
```

2. MRAA lib

```bash
git clone https://github.com/eclipse/mraa
cd mraa
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig

# test
mraa-gpio version
# xxx on Raspberry Pi 4 Model B
```

3. pip install
```bash
pip install numpy pybind11
```

4. other drivers
```bash
# CAN controller driver
sudo apt install can-utils

# python canalystii
pip install python-can canalystii
```

## 编译运行

编译生成cpp test和python依赖库(.so文件)

```bash
mkdir build
cd build 
cmake .. -DBUILD_BASIC_IO=ON -DBUILD_DEVICE_TEST=OFF # default
make

# test
sudo ./test/test_debugger
sudo python3 test_basic_io.py
```

## 支持IO及设备

基本IO的c++库位于include/lib_io.h,其python接口的定义位于src/py_module.cpp中。

- **GPIO basic**
- **UART basic**
    - IMU
- **RS485 basic**
    - dynamixel servo 1.0 (cpp&check;py ing)
    - dynamixel servo 2.0 (cpp&check;)
    - powerboard
- **I2C basic**
    - ADC sensor (cpp&check;)
    - SSD1306 display (cpp&check;)
    - touch board (cpp&check;)
- **SPI basic**
    - ST7735 display (cpp&check;)
    - ST7789 display (cpp&check;)
    - ADC sensor (cpp&check;)
    - encoder
    - nrf24l01
- **CAN basic** (cpp&cross;py&check;)
    - VESC
- others（集成在debugger中）(cpp&check;py ing)
    - rocker
    - button

### 文件结构
```
├── external            # submodule dir
├── include
│   ├── lib_io.h        # basic io lib header
├── src
│   ├── py_module.cpp   # generate python module
│   ├── lib_*.cpp       # basic io libs
│   ├── other cpp       # specific devices libs
├── test
│   ├── basic_io
│   ├── CMakeLists.txt
│   ├── test_*.cpp      # cpp test files
├── pytest
│   ├── src
    ├── basic_io
│   ├── test_*.py       # python test files
├── CMakeLists.txt
├── config.ini          # config file
└── README.md
```

## TODO

- auto detect
- support devices
    - ADC sensor(ADS1115): config register full support

<!-- 使用说明:
    test_uart_ping_new,test_uart_pong_new,test_rs485_ping_new,test_rs485_pong_new添加了几个参数可选配置---串口发送字节,停止位等等全都可选配置,默认是115200的相关配置,如需更改需自行配置,运行相关ping,pong可执行文件时,ping第三个参数uart_delay,即串口发送完成时间115200下推荐1200us的延迟. -->
