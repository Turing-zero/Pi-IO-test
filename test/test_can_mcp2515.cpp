#include "mcp2515.h"
#include <chrono>
#include <thread>
#include <array>

std::array<uint8_t, 4> wheel_can_id = {10, 20, 30, 40};

void set_wheels_rpm(VESC& can, int rpm) {
    for (uint8_t id : wheel_can_id) {
        if (!can.send_vesc_rpm(id, rpm)) {
            std::cerr << "Failed to send VESC RPM" << std::endl;
            return;
        }
    }
}

int main() {
    try {
        VESC can("can0");

        if (!can.begin()) {
            std::cerr << "Failed to initialize CAN interface" << std::endl;
            return 1;
        }
        for (int i = 0; i < 500; i++) {
            int rpm = i * 10;
            set_wheels_rpm(can, rpm);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        for (int i = 500;i >= 0; i--) {
            int rpm = i * 10;
            set_wheels_rpm(can, rpm);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}