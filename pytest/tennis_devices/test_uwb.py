import serial
import time
import numpy as np
from uwb_protocol import *

def serial_init(port: str = '/dev/ttyUSB0', baudrate: int = 115200, timeout: float = 0.01) -> serial.Serial:
    """
    初始化串口连接
    :param baudrate: 波特率
    :param port: 串口名称
    :param timeout: 超时时间
    :return: 串口对象
    """
    ser = serial.Serial()
    ser.port = port
    ser.baudrate = baudrate
    ser.bytesize = 8
    ser.stopbits = 1
    ser.parity = "N"
    ser.timeout = timeout

    try:
        ser.open()
        if ser.isOpen():
            print(f"串口 {port} 打开成功！")
        else:
            print(f"串口 {port} 打开失败！")
            raise Exception("串口打开失败")
    except Exception as e:
        print(f"串口打开异常: {e}")
        raise

    return ser

if __name__ == "__main__":
    # ser = serial_init(port='/dev/ttyUSB1', baudrate=115200, timeout=0.01) # TWR UART2
    ser = serial_init(port='/dev/ttyACM0', baudrate=115200, timeout=0.01) # TWR USB
    
    try:
        invalid_count = 0
        while True:
            data = ser.readline()
            decoded_data = decode_data(data)
            if decoded_data is not None:
                invalid_count = 0
                if decoded_data['protocol'] == 'TWR-USB':
                    print("id: {}, anc_id: {}, seq: {}, mask: {}".format(
                        decoded_data['tag_id'],
                        decoded_data['anc_id'],
                        decoded_data['seq'],
                        decoded_data['mask']
                    ))
                    print("raw distance #0: {:.2f}m, #1: {:.2f}m, #2: {:.2f}m".format(
                        decoded_data['raw_ranges'][0] / 1000.0,
                        decoded_data['raw_ranges'][1] / 1000.0,
                        decoded_data['raw_ranges'][2] / 1000.0
                    ))
                print("distance #0: {:.2f}m, #1: {:.2f}m, #2: {:.2f}m".format(
                    decoded_data['kalman_ranges'][0] / 1000.0,
                    decoded_data['kalman_ranges'][1] / 1000.0,
                    decoded_data['kalman_ranges'][2] / 1000.0
                ))
            else:
                invalid_count = invalid_count + 1
            if invalid_count > 500:
                ser.close()
                raise Exception("!!! disconnected !!!")

    except KeyboardInterrupt:
        pass
    finally:
        ser.close()

