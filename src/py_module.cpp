#include "debugger.h"
#include "rpi_lib.h"
#include "dynamixel2.0.h"
#include "dynamixel1.0.h"
#include <pybind11/pybind11.h>

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
        .value("UART5", UART5);
    py::enum_<mraa::UartParity>(m, "Parity")
        .value("NONE", mraa::UartParity::UART_PARITY_NONE)
        .value("EVEN", mraa::UartParity::UART_PARITY_EVEN)
        .value("ODD", mraa::UartParity::UART_PARITY_ODD)
        .export_values();
    py::class_<uart_module>(m, "uart_module")
        .def(py::init<int, Uart_Port, int, mraa::UartParity, int, bool, bool>())
        .def("open_uart", &uart_module::open_uart)
        .def("close_uart", &uart_module::close_uart)
        .def("send_packet", &uart_module::send_packet)
        .def("recv_packet", &uart_module::recv_packet);

    // rs485
    py::class_<rs485_module>(m, "rs485_module")
        .def(py::init<int, int, Uart_Port, int, int, mraa::UartParity, int, bool, bool>())
        .def("open_rs485", &rs485_module::open_rs485)
        .def("close_rs485", &rs485_module::close_rs485)
        .def("send_485packet", &rs485_module::send_485packet)
        .def("recv_485packet", &rs485_module::recv_485packet);

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
        .def("read", &Dynamixel_1::read)
        .def("write", &Dynamixel_1::write)
        .def("regwrite", &Dynamixel_1::regwrite)
        .def("action", &Dynamixel_1::action)
        .def("bulk_read", &Dynamixel_1::bulk_read)
        .def("sync_write", &Dynamixel_1::sync_write)
        .def("reboot", &Dynamixel_1::reboot)
        .def("factory_reset", &Dynamixel_1::factory_reset)
        .def("get_message", &Dynamixel_1::get_message);

    //dynamixel2.0
    py::class_<Dynamixel_2>(m,"Dynamixel_2")
        .def(py::init())
        .def("open_dynamixel", &Dynamixel_2::open_dynamixel)
        .def("action_angle", &Dynamixel_2::action_angle);
}