# import serial#导入串口通信库
# import time
# import numpy as np


# def config_can_AT(ser: serial.Serial, baudrate: int = 500000):
#     """
#     配置CAN AT命令
#     :param ser: 串口对象
#     :param baudrate: 波特率
#     """
#     ser.open()
#     if(ser.isOpen()):
#         print("串口打开成功！")
#     else:
#         print("串口打开失败！")

#     ser.write("AT+CG\r\n".encode())  # 发送CG命令
#     time.sleep(0.05)
#     ser.write("AT+CAN_BAUD=500000\r\n".encode())  # 设置波特率
#     time.sleep(0.05)

#     ser.write("AT+CAN_BAUD=?\r\n".encode())  # 发送CAN数据
#     time.sleep(0.05)
#     str = ser.read_all()
#     if str:
#         print("Received:", str)
#     else:
#         print("No data received")
#     ser.write("AT+AT\r\n".encode())  # 发送AT命令

# def decode_can_frame(pack):
#     header = b'AT'
#     if header != pack[:2]:
#         raise ValueError("Invalid CAN frame header")
#     id = (int(pack[2]) << 21) | (int(pack[3]) << 13) | (int(pack[4]) << 5) | (int(pack[5]) >> 3)
#     frame_format = (int(pack[5]) & 0x07)  # 0 for standard, 1 for extended
#     length = int(pack[6])
#     data = [int(x) for x in pack[7:15]]
#     print("Received CAN frame id: {}, len: {}, data: {}".format(id, length, data))

# def encode_can_frame(id, length, data, extended=True):
#     id_bytes = [(id >> 21) & 0xFF, (id >> 13) & 0xFF, (id >> 5) & 0xFF, (id << 3) & 0xFF | (0x04 if extended else 0x00)]
#     if len(data) > 8:
#         raise ValueError("Data must be 8 bytes long")
#     frame = b'AT' + bytes(id_bytes) + bytes([length]) + bytes(data) + b'\r\n'
#     return frame

# def encode_rpm_pack(_id, _rpm):
#     id = _id + 0x300
#     data = [0, 0, 0, 0, 0, 0, 0, 0]
#     rpm_int = np.uint32(int(_rpm))
#     data[0] = (rpm_int >> 24) & 0xff
#     data[1] = (rpm_int >> 16) & 0xff
#     data[2] = (rpm_int >> 8) & 0xff
#     data[3] = rpm_int & 0xff
#     frame = encode_can_frame(id, 8, data, extended=True)
#     return frame

# if __name__ == "__main__":
#     ser = serial.Serial()
#     ser.port='/dev/ttyUSB0'
#     ser.baudrate=115200
#     ser.bytesize=8
#     ser.stopbits=1
#     ser.parity="N"#奇偶校验位
#     ser.timeout = 0.01
#     config_can_AT(ser, 500000)  # 配置CAN AT命令

#     can_id_list = [31, 32, 33]
#     rpm_list = [0, 1000, 1200, 1300, 1400, 1500, 1600, 1700, 0]
#     wheel_rpm_list = [0, 500, 600, 900, 1200, 1500, 2000, 0]
#     for rpm in wheel_rpm_list:
#         for id in can_id_list:
#             rpm_frame = encode_rpm_pack(id, rpm)
#             ser.write(rpm_frame)
#         # str = ser.readline()
#         # if str:
#         #     try:
#         #         decode_can_frame(str)
#         #     except ValueError as e:
#         #         print(e)
#         time.sleep(1)
        
#     ser.close()

from .base import CANInterfaceBase
import serial
import numpy as np
import time

class CANUsbWit(CANInterfaceBase):
    """
    CAN Interface class for handling CAN communication.
    This class extends the base CAN interface and implements specific methods for CAN communication.
    """

    def __init__(self, port: str, port_timeout: float = 0.01, baudrate: int = 500000):
        # super().__init__(name)
        self.baudrate = baudrate

        self.ser = serial.Serial()
        self.ser.port = port
        self.ser.baudrate = 115200
        self.ser.bytesize = 8
        self.ser.stopbits = 1
        self.ser.parity = "N"
        self.ser.timeout = port_timeout
        self.open()

    def open(self):
        """Open the CAN interface."""
        self.ser.open()
        if(self.ser.isOpen()):
            print("串口打开成功！")
        else:
            print("串口打开失败！")
        self.ser.write("AT+CG\r\n".encode())  # 发送CG命令
        time.sleep(0.05)
        self.ser.write(f"AT+CAN_BAUD={self.baudrate}\r\n".encode())  # 设置波特率
        time.sleep(0.05)

        self.ser.write("AT+CAN_BAUD=?\r\n".encode())  # 发送CAN数据
        time.sleep(0.05)
        str = self.ser.read_all()
        if str:
            print("Received:", str)
        else:
            print("No data received")
        self.ser.write("AT+AT\r\n".encode())  # 发送AT命令

    def close(self):
        """Close the CAN interface."""
        if self.ser.is_open:
            self.ser.close()

    def send(self, _id, _data, extended=True):
        """
        Send a message over the CAN interface.
        
        Parameters:
        _id (int): The CAN ID of the message.
        _data (list): The data to be sent in the message.
        """
        id_bytes = [(_id >> 21) & 0xFF, (_id >> 13) & 0xFF, (_id >> 5) & 0xFF, (_id << 3) & 0xFF | (0x04 if extended else 0x00)]

        frame = b'AT' + bytes(id_bytes) + bytes([len(_data)]) + bytes(_data) + b'\r\n'

        self.ser.write(frame)
    
    def receive(self, timeout=None):
        # self.ser.timeout = timeout
        data_bytes = self.ser.readline()
        try :
            if data_bytes.startswith(b'AT'):
                id = (int(data_bytes[2]) << 21) | (int(data_bytes[3]) << 13) | (int(data_bytes[4]) << 5) | (int(data_bytes[5]) >> 3)
                length = int(data_bytes[6])
                data = [int(x) for x in data_bytes[7:7+length]]
                return id, data
            else:
                raise ValueError("Invalid CAN frame header")
        except Exception as e:
            # print(f"Error decoding CAN frame: {e}")
            return None, None
