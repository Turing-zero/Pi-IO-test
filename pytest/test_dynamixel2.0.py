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

# id = 4
for i in range(4,5):
    recv_buf = np.zeros(25, dtype=np.uint8)
    test.dir_write(i, recv_buf, 112, 10, 4)
    print(recv_buf)
    test.read(i, recv_buf, 112,4, 0)
    print(recv_buf)
# recv_buf = np.zeros(25, dtype=np.uint8)
# position = test.get_present_position_deg(id, recv_buf)
# print(recv_buf)
# recv_buf = np.zeros(25, dtype=np.uint8)
# speed = test.get_present_speed_rpm(id, recv_buf)
# print(recv_buf)
# print("Position: {}, Speed: {}".format(position, speed))

# for i in range(4):
#     recv_buf = np.zeros(25, dtype=np.uint8)
#     position = test.get_present_position_deg(i, recv_buf)
#     print("ID:",i,"-->",position)
# while True:
#     recv_buf = np.zeros(25, dtype=np.uint8)
#     position = test.get_present_position(0, recv_buf)
#     # print(recv_buf)
#     position_deg = test.get_present_position_deg(0, recv_buf)
#     print("Int0 -->",position)
#     print("Deg0 -->",position_deg)

# for i in range(360):
#     test.set_goal_position_deg(4, 2*i-90)
# test.set_goal_position_deg(4, 400)
    # time.sleep(1)
# time.sleep(1)
# test.set_goal_position_deg(id, 180)
# test.set_torque_enable(4, False)
# 0 [450 270] 360
# 1 [35 120] 35
# 2 [15 180] 100
# 3 [140 325] 235
# 4 [0 360] 180
angle_init = []
def position_init():
    for i in range(4):
        recv_buf = np.zeros(25, dtype=np.uint8)
        angle_init.append(int(test.get_present_position_deg(i, recv_buf)))
    for i in range(4):
        # test.set_goal_position_deg(i, angle_init[i])
        print("ID:",i,"-->",angle_init[i])
    # test.set_goal_position_deg(4, angle_init[4]+50)
    # test.set_goal_position_deg(0, 360)
    # test.set_goal_position_deg(1, 35)
    # test.set_goal_position_deg(2, 100)
    # test.set_goal_position_deg(3, 235)
    # test.set_goal_position_deg(4, 180)
    # test.set_torque_enable(id, true)
def relax_all():
    for i in range(5):
        test.set_torque_enable(i, False)

# position_init()
# time.sleep(20)
# relax_all()
# recv_buf = np.zeros(25, dtype=np.uint8)
# position = int(test.get_present_position_deg(4, recv_buf))
# print("Int0 -->",position)
# test.set_goal_position_deg(4, position-30)
# test.set_goal_position_deg(0, 370)
# for i in range(4):
#     print(angle_init[i])
test.set_goal_position_deg(4, -30)