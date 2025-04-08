#ifndef __DYNAMIXEL_1__
#define __DYNAMIXEL_1__
#include "lib_io.h"
#include <cmath>

class Dynamixel_1{
    public:
        void open_dynamixel(int baudrate = 1000000,int uart_delay = 400,Uart_Port port = UART0,int control_pin=36);

        void set_goal_position(int id,uint16_t position);
        void set_goal_position_deg(int id, int deg);
        void set_moving_speed(int id, uint16_t speed);
        void set_moving_speed_rpm(int id, int rpm);

        void action_angle(int id,double angle);
        int factory_reset(int id,char* recv_buf);
        int ping(int id,char* recv_buf);
        int read(int id,char* recv_buf,int address,int size);
        int write(int id,char* recv_buf,int address,int data);
        int regwrite(int id,char* recv_buf,int address,int data);
        int action(int id,char* recv_buf);
        int bulk_read(char* recv_buf,int*id_group,int*address,int*data_size,int size);
        void sync_write(int*id_group,long int*data,int size,int address,int datasize);
        int reboot(int id,char* recv_buf);
    private:
        int angle2hex(double angle);
        rs485_module *dynamixel;  
};


namespace MX64_EEPROM {
    // Address 0-1: Model Number (Read-Only)
    const uint8_t MODEL_NUMBER_L = 0;
    const uint8_t MODEL_NUMBER_H = 1;

    // Address 2: Firmware Version (Read-Only)
    const uint8_t FIRMWARE_VERSION = 2;

    // Address 3: ID (Read-Write, Default=1)
    const uint8_t ID = 3;

    // Address 4: Baud Rate (Read-Write, Default=34)
    const uint8_t BAUD_RATE = 4;

    // Address 5: Return Delay Time (Read-Write, Default=250)
    const uint8_t RETURN_DELAY = 5;

    // Address 6-7: CW Angle Limit (Read-Write, Default=0)
    const uint8_t CW_ANGLE_LIMIT_L = 6;
    const uint8_t CW_ANGLE_LIMIT_H = 7;

    // Address 8-9: CCW Angle Limit (Read-Write, Default=4095)
    const uint8_t CCW_ANGLE_LIMIT_L = 8;
    const uint8_t CCW_ANGLE_LIMIT_H = 9;

    // Address 11: Temperature Limit (Read-Write, Default=80)
    const uint8_t TEMP_LIMIT = 11;

    // Address 12: Min Voltage Limit (Read-Write, Default=60)
    const uint8_t MIN_VOLTAGE = 12;

    // Address 13: Max Voltage Limit (Read-Write, Default=240)
    const uint8_t MAX_VOLTAGE = 13;

    // Address 14-15: Max Torque (Read-Write, Default=1023)
    const uint8_t MAX_TORQUE_L = 14;
    const uint8_t MAX_TORQUE_H = 15;

    // Address 16: Status Return Level (Read-Write, Default=2)
    const uint8_t STATUS_RETURN_LEVEL = 16;

    // Address 17: Alarm LED (Read-Write, Default=36)
    const uint8_t ALARM_LED = 17;

    // Address 18: Shutdown (Read-Write, Default=36)
    const uint8_t SHUTDOWN = 18;

    // Address 20-21: Multi-Turn Offset (Read-Write, Default=0)
    const uint8_t MULTI_TURN_OFFSET_L = 20;
    const uint8_t MULTI_TURN_OFFSET_H = 21;

    // Address 22: Resolution Divider (Read-Write, Default=1)
    const uint8_t RESOLUTION_DIVIDER = 22;
};

namespace MX64_RAM {
    // Address 24: Torque Enable (Read-Write, Default=0)
    const uint8_t TORQUE_ENABLE = 24;

    // Address 25: LED (Read-Write, Default=0)
    const uint8_t LED = 25;

    // Address 26: D Gain (Read-Write, Default=0)
    const uint8_t D_GAIN = 26;

    // Address 27: I Gain (Read-Write, Default=0)
    const uint8_t I_GAIN = 27;

    // Address 28: P Gain (Read-Write, Default=32)
    const uint8_t P_GAIN = 28;

    // Address 30-31: Goal Position (Read-Write)
    const uint8_t GOAL_POSITION = 30;

    // Address 32-33: Moving Speed (Read-Write)
    const uint8_t MOVING_SPEED = 32;

    // Address 34-35: Torque Limit (Read-Write)
    const uint8_t TORQUE_LIMIT_L = 34;
    const uint8_t TORQUE_LIMIT_H = 35;

    // Address 36-37: Present Position (Read-Only)
    const uint8_t PRESENT_POSITION_L = 36;
    const uint8_t PRESENT_POSITION_H = 37;

    // Address 38-39: Present Speed (Read-Only)
    const uint8_t PRESENT_SPEED_L = 38;
    const uint8_t PRESENT_SPEED_H = 39;

    // Address 40-41: Present Load (Read-Only)
    const uint8_t PRESENT_LOAD_L = 40;
    const uint8_t PRESENT_LOAD_H = 41;

    // Address 42: Present Voltage (Read-Only)
    const uint8_t PRESENT_VOLTAGE = 42;

    // Address 43: Present Temperature (Read-Only)
    const uint8_t PRESENT_TEMP = 43;

    // Address 44: Registered (Read-Only)
    const uint8_t REGISTERED = 44;

    // Address 46: Moving (Read-Only)
    const uint8_t MOVING = 46;

    // Address 47: Lock EEPROM (Read-Write)
    const uint8_t LOCK = 47;

    // Address 48-49: Punch (Read-Write)
    const uint8_t PUNCH_L = 48;
    const uint8_t PUNCH_H = 49;

    // Address 50-51: Realtime Tick (Read-Only)
    const uint8_t REALTIME_TICK_L = 50;
    const uint8_t REALTIME_TICK_H = 51;

    // Address 68-69: Current (Read-Write)
    const uint8_t CURRENT_L = 68;
    const uint8_t CURRENT_H = 69;

    // Address 70: Torque Control Mode (Read-Write)
    const uint8_t TORQUE_CTRL_MODE = 70;

    // Address 71-72: Goal Torque (Read-Write)
    const uint8_t GOAL_TORQUE_L = 71;
    const uint8_t GOAL_TORQUE_H = 72;

    // Address 73: Goal Acceleration (Read-Write)
    const uint8_t GOAL_ACCEL = 73;
};

#endif