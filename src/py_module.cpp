#include "debugger.h"
#include "lib_io.h"
#include "dynamixel2.0.h"
#include "dynamixel1.0.h"
#include "sensor_adc.h"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
PYBIND11_MODULE(debugger, m) {
    // debugger
    py::class_<Debugger>(m, "Debugger")
        .def(py::init())
        .def("open_device", &Debugger::open_device)
        .def("get_value", &Debugger::get_value)
        .def("get_status", &Debugger::get_status);
    py::enum_<Device>(m, "Device")
        .value("DYNAMIXEL", DYNAMIXEL)
        .value("ROCKER", ROCKER)
        .value("IMU", IMU)
        .value("BUTTON", BUTTON);

    // gpio
    py::class_<gpio_module>(m, "gpio_module")
        .def(py::init<int, mraa::Dir>())
        .def("write", &gpio_module::write)
        .def("read", &gpio_module::read)
        .def("close", &gpio_module::close);
    py::enum_<mraa::Dir>(m, "Dir")
        .value("IN", mraa::Dir::DIR_IN)
        .value("OUT", mraa::Dir::DIR_OUT);

    // uart
    py::enum_<Uart_Port>(m, "Uart_Port")
        .value("UART0", UART0)
        .value("UART1", UART1)
        .value("UART2", UART2)
        .value("UART3", UART3)
        .value("UART4", UART4)
        .value("UART5", UART5)
        .value("CUSTOM", CUSTOM);
    py::enum_<mraa::UartParity>(m, "Parity")
        .value("NONE", mraa::UartParity::UART_PARITY_NONE)
        .value("EVEN", mraa::UartParity::UART_PARITY_EVEN)
        .value("ODD", mraa::UartParity::UART_PARITY_ODD)
        .export_values();
    py::class_<uart_module>(m, "uart_module")
        .def(py::init<int, Uart_Port, int, mraa::UartParity, int, bool, bool>())
        .def("open_uart", &uart_module::open_uart)
        .def("close_uart", &uart_module::close_uart)
        .def("send_packet",[](uart_module &self,py::array_t<char> packet,int length){
            py::buffer_info buf = packet.request();
            char* ptr = (char*)buf.ptr;
            self.send_packet(ptr,length);
            },py::arg("packet"),py::arg("length"))
        .def("recv_packet", &uart_module::recv_packet);

    // rs485
    py::class_<rs485_module>(m, "rs485_module")
        .def(py::init<int, int, Uart_Port, int, int, mraa::UartParity, int, bool, bool>())
        .def("open_rs485", &rs485_module::open_rs485)
        .def("close_rs485", &rs485_module::close_rs485)
        .def("send_485packet",[](rs485_module &self,py::array_t<char> packet,int length){
            py::buffer_info buf = packet.request();
            char* ptr = (char*)buf.ptr;
            self.send_485packet(ptr,length);
            },py::arg("packet"),py::arg("length"))
        .def("recv_485packet",[](rs485_module &self,py::array_t<char> packet,int length){
            py::buffer_info buf = packet.request();
            char* ptr = (char*)buf.ptr;
            self.recv_485packet(ptr,length);
            },py::arg("packet"),py::arg("length"));

    // spi
    py::class_<spi_module>(m, "spi_module")
        .def(py::init<int, int, int, int, int, bool>())
        .def("open_spi", &spi_module::open_spi)
        .def("close_spi", &spi_module::close_spi)
        .def("transfer", &spi_module::transfer)
        .def("write", &spi_module::write)
        .def("transfer_word", &spi_module::transfer_word)
        .def("write_word", &spi_module::write_word);

    // i2c
    py::class_<i2c_module>(m, "i2c_module")
        .def(py::init<int>())
        .def("open_i2c", &i2c_module::open_i2c)
        .def("write", &i2c_module::write)
        .def("read", &i2c_module::read)
        .def("writeByte", &i2c_module::writeByte)
        .def("readByte", &i2c_module::readByte)
        .def("readBytesReg", &i2c_module::readBytesReg)
        .def("close_i2c", &i2c_module::close_i2c);

    //dynamixel1.0
    py::class_<Dynamixel_1>(m,"Dynamixel_1")
        .def(py::init())
        .def("open_dynamixel", &Dynamixel_1::open_dynamixel)
        .def("action_angle", &Dynamixel_1::action_angle)
        .def("ping", &Dynamixel_1::ping)
        .def("read",[](Dynamixel_1 &self,int id,py::array_t<char> recv_buf,int address,int size){
            py::buffer_info buf = recv_buf.request();
            char* ptr = (char*)buf.ptr;
            self.read(id,ptr,address,size);
            },py::arg("id"),py::arg("recv_buf"),py::arg("address"),py::arg("size"))
        .def("write", &Dynamixel_1::write)
        .def("regwrite", &Dynamixel_1::regwrite)
        .def("action", &Dynamixel_1::action)
        .def("bulk_read", &Dynamixel_1::bulk_read)
        .def("sync_write",[](Dynamixel_1 &self,py::array_t<int> id_group,py::array_t<long int> data,int size,int address,int datasize){
            py::buffer_info ids = id_group.request();
            py::buffer_info datas = data.request();
            int* idptr = (int*)ids.ptr;
            long int* dataptr = (long int*)datas.ptr;
            self.sync_write(idptr,dataptr,size,address,datasize);
            },py::arg("id_group"),py::arg("data"),py::arg("size"),py::arg("address"),py::arg("datasize"))
        .def("reboot",[](Dynamixel_1 &self,int id,py::array_t<char> recv_buf){
            py::buffer_info buf = recv_buf.request();
            char* ptr = (char*)buf.ptr;
            self.reboot(id,ptr);
            },py::arg("id"),py::arg("recv_buf"))
        .def("factory_reset",[](Dynamixel_1 &self,int id,py::array_t<char> recv_buf){
            py::buffer_info buf = recv_buf.request();
            char* ptr = (char*)buf.ptr;
            self.factory_reset(id,ptr);
            },py::arg("id"),py::arg("recv_buf"))
        .def("set_goal_position", &Dynamixel_1::set_goal_position)
        .def("set_goal_position_deg", &Dynamixel_1::set_goal_position_deg)
        .def("set_moving_speed", &Dynamixel_1::set_moving_speed)
        .def("set_moving_speed_rpm", &Dynamixel_1::set_moving_speed_rpm);

    //dynamixel2.0
    py::class_<Dynamixel_2>(m,"Dynamixel_2")
        .def(py::init())
        .def("open_dynamixel", &Dynamixel_2::open_dynamixel)
        .def("action_angle", &Dynamixel_2::action_angle)
        .def("ping", [](Dynamixel_2 &self,int id,py::array_t<char> recv_buf){
            py::buffer_info buf = recv_buf.request();
            char* ptr = (char*)buf.ptr;
            self.ping(id,ptr);
            },py::arg("id"),py::arg("recv_buf"))
        .def("read",[](Dynamixel_2 &self,int id,py::array_t<char> recv_buf,int address,int low_size,int high_size){
            py::buffer_info buf = recv_buf.request();
            char* ptr = (char*)buf.ptr;
            self.read(id,ptr,address,low_size,high_size);
            },py::arg("id"),py::arg("recv_buf"),py::arg("address"),py::arg("low_size"),py::arg("high_size"))
        .def("write", &Dynamixel_2::write)
        .def("regwrite", &Dynamixel_2::regwrite)
        .def("action", &Dynamixel_2::action)
        .def("set_torque_enable", &Dynamixel_2::set_torque_enable)
        .def("set_goal_position", &Dynamixel_2::set_goal_position)
        .def("set_goal_position_deg", &Dynamixel_2::set_goal_position_deg)
        .def("set_goal_vel", &Dynamixel_2::set_goal_vel)
        .def("set_goal_vel_rpm", &Dynamixel_2::set_goal_vel_rpm)
        .def("get_present_position", [](Dynamixel_2 &self,int id,py::array_t<char> recv_buf){
            py::buffer_info buf = recv_buf.request();
            char* ptr = (char*)buf.ptr;
            return self.get_present_position(id,ptr);
            },py::arg("id"),py::arg("recv_buf"))
        .def("get_present_position_deg", [](Dynamixel_2 &self,int id,py::array_t<char> recv_buf){
            py::buffer_info buf = recv_buf.request();
            char* ptr = (char*)buf.ptr;
            return self.get_present_position_deg(id,ptr);
            },py::arg("id"),py::arg("recv_buf"))
        .def("get_present_speed", [](Dynamixel_2 &self,int id,py::array_t<char> recv_buf){
            py::buffer_info buf = recv_buf.request();
            char* ptr = (char*)buf.ptr;
            return self.get_present_speed(id,ptr);
            },py::arg("id"),py::arg("recv_buf"))
        .def("get_present_speed_rpm", [](Dynamixel_2 &self,int id,py::array_t<char> recv_buf){
            py::buffer_info buf = recv_buf.request();
            char* ptr = (char*)buf.ptr;
            return self.get_present_speed_rpm(id,ptr);
            },py::arg("id"),py::arg("recv_buf"));

    //sensor_adc
    py::enum_<ADS1115_DATA_RATE>(m, "ADS1115_DATA_RATE")
        .value("DR_8SPS", DR_8SPS)
        .value("DR_16SPS", DR_16SPS)
        .value("DR_32SPS", DR_32SPS)
        .value("DR_64SPS", DR_64SPS)
        .value("DR_128SPS", DR_128SPS)
        .value("DR_250SPS", DR_250SPS)
        .value("DR_475SPS", DR_475SPS)
        .value("DR_860SPS", DR_860SPS);
    py::enum_<ADS1115_PGA>(m, "ADS1115_PGA")
        .value("PGA_6_144V", PGA_6_144V)
        .value("PGA_4_096V", PGA_4_096V)
        .value("PGA_2_048V", PGA_2_048V)
        .value("PGA_1_024V", PGA_1_024V)
        .value("PGA_0_512V", PGA_0_512V)
        .value("PGA_0_256V", PGA_0_256V);
    py::class_<Sensor_ADC>(m, "Sensor_ADC")
        .def(py::init())
        .def("open_sensor", &Sensor_ADC::open_sensor)
        .def("get_voltage_mux", &Sensor_ADC::get_voltage_mux)
        .def("config_data_rate", &Sensor_ADC::config_data_rate)
        .def("config_pga", &Sensor_ADC::config_pga);
}