import sys
import configparser
sys.path.append("../build/")

import debugger

config = configparser.ConfigParser()
config.read('../config.ini')

test = debugger.Dynamixel()

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


baudrate   = int(config['dynamixel']['baudrate'])
uart_delay = int(config['dynamixel']['uart_delay'])
uart_port  = get_uart_port(int(config['dynamixel']['uart_port']))
control_pin= int(config['dynamixel']['control_pin'])

test.open_dynamixel(baudrate,uart_delay,uart_port,control_pin)
test.action_angle(1,3.14)