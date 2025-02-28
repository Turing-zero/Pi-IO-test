#include "mraa.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include "lib_io.h"
#include "sensor_adc.h"

int main() {
    std::ofstream outFile("../temp/voltages.txt");
    if (!outFile) {
        std::cerr << "cannot open file!" << std::endl;
    }

    Sensor_ADC sensor;
    sensor.open_sensor(0);
    while (1) {
        std::vector<float> voltages;
        for (int device = 0; device < 4; device++) {
            for (int channel = 0; channel < 2; channel++) {
                float voltage = sensor.get_voltage_mux(device, channel);
                voltages.emplace_back(voltage);
            }
        }
        for (int i = 0; i < voltages.size(); i++) {
            printf("%5.2f  ", voltages[i]);
            outFile << voltages[i] << " ";
        }
        printf("\n");
        outFile << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    outFile.close();
    return 0;
}