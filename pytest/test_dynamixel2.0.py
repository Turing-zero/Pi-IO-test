import sys
import configparser
import numpy as np
import time
sys.path.append("../build/")

import debugger

config = configparser.ConfigParser()
config.read('../config.ini')

test = debugger.Dynamixel_2()

def get_uart_port(port):
    if port == 1:
        return debugger.Uart_Port.UART1
    elif port == 2:
        return debugger.Uart_Port.UART2
    elif port == 3:
        return debugger.Uart_Port.UART3
    elif port == 4:
        return debugger.Uart_Port.UART4
    elif port == 5:
        return debugger.Uart_Port.UART5
    elif port == 6:
        return debugger.Uart_Port.CUSTOM
    else:
        return debugger.Uart_Port.UART0

baudrate   = int(config['dynamixel']['baudrate'])
uart_delay = int(config['dynamixel']['uart_delay'])
uart_port  = get_uart_port(int(config['dynamixel']['uart_port']))
control_pin= int(config['dynamixel']['control_pin'])

test.open_dynamixel(baudrate,uart_delay,uart_port,control_pin)

id = 4

# recv_buf = np.zeros(25, dtype=np.uint8)
# test.read(id, recv_buf, 144, 2, 0)
# print(recv_buf)
# recv_buf = np.zeros(25, dtype=np.uint8)
# position = test.get_present_position_deg(id, recv_buf)
# print(recv_buf)
# recv_buf = np.zeros(25, dtype=np.uint8)
# speed = test.get_present_speed_rpm(id, recv_buf)
# print(recv_buf)
# print("Position: {}, Speed: {}".format(position, speed))

for i in range(5):
    recv_buf = np.zeros(25, dtype=np.uint8)
    position = test.get_present_position_deg(i, recv_buf)
    print("ID: ",i,recv_buf)


# test.set_goal_position_deg(id, 0)
# time.sleep(1)
# test.set_goal_position_deg(id, 180)