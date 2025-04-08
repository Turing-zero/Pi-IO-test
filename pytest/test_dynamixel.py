import sys
import configparser
import numpy as np
import time
sys.path.append("../build/")

import debugger

config = configparser.ConfigParser()
config.read('../config.ini')

test = debugger.Dynamixel_1()

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
# test.action_angle(2, 3.14)
# data_array = np.array([""]*256)

test.set_goal_position_deg(2, 0)
time.sleep(1)
test.set_goal_position_deg(2, 180)

# id_group = np.array([1, 2], dtype=np.int32)
# data = np.array([1, 1], dtype=np.int32)

# recv_buf = np.array([0,0,0,0,0,0,0,0,0,0],dtype=np.uint8)
# size = test.read(2,recv_buf,0x18,1)
# print(recv_buf)
# test.sync_write(id_group,data,2,0x18,1)
# size = test.read(2,recv_buf,0x18,1)
# # dd = test.get_message()
# print(recv_buf)
