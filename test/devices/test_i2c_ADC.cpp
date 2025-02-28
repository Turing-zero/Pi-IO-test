#include "mraa.hpp"
#include <iostream>
#include <unistd.h>
#include "lib_io.h"
#include "sensor_adc.h"

int main() {
    Sensor_ADC sensor;
    sensor.open_sensor(0);
    while (1) {
        float voltage = sensor.get_voltage_mux(0,0);
        std::cout << "voltage: " << voltage << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}