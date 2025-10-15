import numpy as np
from ctypes import *
import ctypes
import time
from ctypes import *

class VESC_CAN_STATUS:
    VESC_ID_A = 20
    VESC_ID_B = 25
    VESC_CAN_PACKET_STATUS_1 = 0x09
    VESC_CAN_PACKET_STATUS_2 = 0x0E
    VESC_CAN_PACKET_STATUS_3 = 0x0F
    VESC_CAN_PACKET_STATUS_4 = 0x10
    VESC_CAN_PACKET_STATUS_5 = 0x1B

class VESC_PACK(Structure):
    _fields_ = [("id", c_int),
                ("rpm", c_int),
                ("current", c_float),
                ("pid_pos_now", c_float),
                ("amp_hours", c_float),
                ("amp_hours_charged", c_float),
                ("watt_hours", c_float),
                ("watt_hours_charged", c_float),
                ("temp_fet", c_float),
                ("temp_motor", c_float),
                ("tot_current_in", c_float),
                ("duty", c_float),
                ("tachometer_value", c_float),
                ("input_voltage", c_float)
                ]

def buffer_get_int16(buffer, index):
    value = buffer[index] << 8 | buffer[index + 1]
    # 处理16位有符号整数溢出问题
    # 如果最高位是1，则表示这是一个负数
    if value & 0x8000:
        # 转换为负数的补码表示
        value = value - 0x10000
    return value
def buffer_get_int32(buffer, index):
    value = buffer[index] << 24 | buffer[index + 1] << 16 | buffer[index + 2] << 8 | buffer[index + 3]
    # 处理32位有符号整数溢出问题
    # 如果最高位是1，则表示这是一个负数
    if value & 0x80000000:
        # 转换为负数的补码表示
        value = value - 0x100000000
    return value
def buffer_get_float16(buffer, scale, index):
    value = buffer_get_int16(buffer, index)
    return float(value) / scale
def buffer_get_float32(buffer, scale, index):
    value = buffer_get_int32(buffer, index)
    return float(value) / scale

class VESC():
    def __init__(self, can_handle):
        self.can_packet = VESC_PACK()
        self.can_handle = can_handle

    # init default: socketcan(mcp2515)
    # def __init__(self, _interface='socketcan', _channel='can0', _bitrate=500000):
    #     self.bus = can.ThreadSafeBus(interface=_interface, channel=_channel, bitrate=_bitrate, receive_own_messages=False)
    #     self.can_packet = VESC_PACK()                              

    # def __del__(self):
    #     self.bus.shutdown()
    
    # def send(self, id, data):
    #     message = can.Message(arbitration_id=id, is_extended_id=True, data=data)
    #     self.bus.send(message)

    # def send_can_msg(self, msg):
    #     self.bus.send(msg)
    
    def send_pass_through(self, _id:np.uint8, _pos:float, _rpm:float, _cur:float):
        id = _id + 0x3F00
        data = [0, 0, 0, 0, 0, 0, 0, 0]
        # 处理位置值，将其转换为有符号16位整数
        pos_int = int(_pos*100)
        # 确保位置值在16位有符号整数范围内
        if pos_int < -32768:
            pos_int = -32768
        elif pos_int > 32767:
            pos_int = 32767
            
        # 处理RPM值，将其转换为有符号16位整数
        rpm_int = int(_rpm)
        # 确保RPM值在16位有符号整数范围内
        if rpm_int < -32768:
            rpm_int = -32768
        elif rpm_int > 32767:
            rpm_int = 32767
            
        # 处理电流值，将其转换为有符号16位整数
        cur_int = int(_cur*1000)
        # 确保电流值在16位有符号整数范围内
        if cur_int < -32768:
            cur_int = -32768
        elif cur_int > 32767:
            cur_int = 32767
            
        data[0] = (pos_int >> 8) & 0xff
        data[1] = pos_int & 0xff
        data[2] = (rpm_int >> 8) & 0xff
        data[3] = rpm_int & 0xff
        data[4] = (cur_int >> 8) & 0xff
        data[5] = cur_int & 0xff
        # print("SEND vesc id: {}, pos: {}, data: {}".format(id & 0xff, pos_int, data))
        self.can_handle.send(id, data)

    def send_pos(self, _id:np.uint8, _pos:float, usb_channel=0, can_channel=0):
        id = _id + 0x400
        data = [0, 0, 0, 0, 0, 0, 0, 0]
        # 处理位置值，将其转换为有符号32位整数
        pos_int = int(_pos*1e6)
        # 确保位置值在32位有符号整数范围内
        if pos_int < -2147483648:
            pos_int = -2147483648
        elif pos_int > 2147483647:
            pos_int = 2147483647
            
        data[0] = (pos_int >> 24) & 0xff
        data[1] = (pos_int >> 16) & 0xff
        data[2] = (pos_int >> 8) & 0xff
        data[3] = pos_int & 0xff
        print("SEND vesc id: {}, pos: {}, data: {}".format(id & 0xff, pos_int, data))
        self.can_handle.send(id, data)

    def send_rpm(self, _id:np.uint8, _rpm:float):
        id = _id + 0x300
        data = [0, 0, 0, 0, 0, 0, 0, 0]
        # 处理负数RPM值，将其转换为有符号32位整数
        rpm_int = int(_rpm)
        # 确保RPM值在32位有符号整数范围内
        if rpm_int < -2147483648:
            rpm_int = -2147483648
        elif rpm_int > 2147483647:
            rpm_int = 2147483647
            
        data[0] = (rpm_int >> 24) & 0xff
        data[1] = (rpm_int >> 16) & 0xff
        data[2] = (rpm_int >> 8) & 0xff
        data[3] = rpm_int & 0xff
        # print("SEND vesc id: {}, rpm: {}, data: {}".format(id & 0xff, rpm_int, data))
        # self.send(id, data, usb_channel, can_channel)
        self.can_handle.send(id, data)
        # self.bus.send(message, timeout=0.2)
    
    def send_current(self, _id:np.uint8, _cur:float):
        id = _id + 0x100
        data = [0, 0, 0, 0, 0, 0, 0, 0]
        off_delay_int = np.uint16(0)
        # 处理负数电流值，将其转换为有符号32位整数
        cur_int = int(_cur * 1000)
        # 确保电流值在32位有符号整数范围内
        if cur_int < -2147483648:
            cur_int = -2147483648
        elif cur_int > 2147483647:
            cur_int = 2147483647
            
        data[0] = (off_delay_int >> 8) & 0xff
        data[1] = off_delay_int & 0xff
        data[2] = (cur_int >> 24) & 0xff
        data[3] = (cur_int >> 16) & 0xff
        data[4] = (cur_int >> 8) & 0xff
        data[5] = cur_int & 0xff
        # print("SEND vesc id: {}, cur: {}, data: {}".format(id & 0xff, cur_int, data))
        self.can_handle.send(id, data)

    def receive_decode(self,timeout=0.01):
        id, data = self.can_handle.receive(timeout)
        if id is None:
            return None,None

        # 只处理ID为40的消息，其他ID的消息忽略
        # vesc_id = id & 0xff
        # if vesc_id != 40:
        #     print(f"Ignoring message from VESC ID {vesc_id}, only accepting ID 40")
        #     return None, None

        # print("RECV vesc id: {}, data: {}".format(id & 0xff, data))
        # self.can_packet.id = vesc_id
        # uint8_t can_packet_status_id = (msg->identifier >> 8) & 0xff;
        # int32_t send_index = 0;
        status_id = (id >> 8) & 0xff
        if status_id == VESC_CAN_STATUS.VESC_CAN_PACKET_STATUS_1:
            self.can_packet.rpm = int(buffer_get_float32(data, 1, 0))
            self.can_packet.current = buffer_get_float16(data, 1e2, 4)
            self.can_packet.pid_pos_now = buffer_get_float16(data, 50.0, 6)
            print("RECV vesc id: {}, rpm: {}, current: {}, pid_pos_now: {}".format(id & 0xff, self.can_packet.rpm, self.can_packet.current, self.can_packet.pid_pos_now))
        else:
            print("not vesc status packet!")
            return None, None
        if status_id == VESC_CAN_STATUS.VESC_CAN_PACKET_STATUS_2:
            self.can_packet.amp_hours = buffer_get_float32(data, 1e4, 0)
            self.can_packet.amp_hours_charged = buffer_get_float32(data, 1e4, 4)
        if status_id == VESC_CAN_STATUS.VESC_CAN_PACKET_STATUS_3:
            self.can_packet.watt_hours = buffer_get_float32(data, 1e4, 0)
            self.can_packet.watt_hours_charged = buffer_get_float32(data, 1e4, 4)
        if status_id == VESC_CAN_STATUS.VESC_CAN_PACKET_STATUS_4:
            self.can_packet.temp_fet = buffer_get_float16(data, 1e1, 0)
            self.can_packet.temp_motor = buffer_get_float16(data, 1e1, 2)
            self.can_packet.tot_current_in = buffer_get_float16(data, 1e1, 4)
            self.can_packet.duty = buffer_get_float16(data, 1e3, 6)
        if status_id == VESC_CAN_STATUS.VESC_CAN_PACKET_STATUS_5:
            self.can_packet.tachometer_value = buffer_get_float32(data, 1, 0)
            self.can_packet.input_voltage = buffer_get_float16(data, 1e1, 4)
        return id, self.can_packet

    # def receive(self,timeout=0.01):
    #     msg = self.bus.recv(timeout)
    #     if msg is not None:
    #         # print(msg)
    #         return msg.arbitration_id, msg.data
    #     else:
    #         return None, None
