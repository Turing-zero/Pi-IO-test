import sys
import configparser
import numpy as np
sys.path.append("../build/")

import debugger

#uart1 是minibluetooth的
'''
debugger.Uart_Port.UART0 对应 /dev/ttyAMA0
debugger.Uart_Port.UART1 对应 /dev/ttys0
debugger.Uart_Port.UART2 对应 /dev/ttyAMA1
以此类推
'''

#rs485
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
    else:
        return debugger.Uart_Port.UART0

baudrate = 1000000
uart_delay = 150
control_pin = 38
databyte=8
stopbit = 1
parity=debugger.Parity.NONE
xonxoff=False
rtscts=False

test = debugger.rs485_module(baudrate,uart_delay,get_uart_port(2),control_pin,databyte,parity,stopbit,xonxoff,rtscts)
test.open_rs485()
data = np.array([255,2,3,4,5],dtype=np.uint8)

#发送函数
test.send_485packet(data,5)

#接收函数需要一直开着，等着接收发送端发来的消息
# while True:
#     test.recv_485packet(data,10)

#     print(data)

#uart

