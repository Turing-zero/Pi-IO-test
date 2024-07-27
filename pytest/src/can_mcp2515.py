import can
import numpy as np

class CAN_MCP2515:
    def __init__(self, baudrate='500k'):
        self.bus = can.ThreadSafeBus(interface='socketcan', channel='can0', receive_own_messages=False)
    
    def send(self, id, data):
        message = can.Message(arbitration_id=id, is_extended_id=True, data=data)
        self.bus.send(message)

    def send_can_msg(self, msg):
        self.bus.send(msg)
    
    def send_pos(self, _id:np.uint8, _pos:float, usb_channel=0, can_channel=0):
        id = _id + 0x400
        data = [0, 0, 0, 0, 0, 0, 0, 0]
        pos_int = np.uint32(int(_pos*1e6))
        data[0] = (pos_int >> 24) & 0xff
        data[1] = (pos_int >> 16) & 0xff
        data[2] = (pos_int >> 8) & 0xff
        data[3] = pos_int & 0xff
        print("SEND vesc id: {}, pos: {}, data: {}".format(id & 0xff, pos_int, data))
        self.send(id, data, usb_channel, can_channel)

    def send_rpm(self, _id:np.uint8, _rpm:float):
        id = _id + 0x300
        data = [0, 0, 0, 0, 0, 0, 0, 0]
        rpm_int = np.uint32(int(_rpm))
        data[0] = (rpm_int >> 24) & 0xff
        data[1] = (rpm_int >> 16) & 0xff
        data[2] = (rpm_int >> 8) & 0xff
        data[3] = rpm_int & 0xff
        # print("SEND vesc id: {}, rpm: {}, data: {}".format(id & 0xff, rpm_int, data))
        # self.send(id, data, usb_channel, can_channel)
        self.send(id, data)
        # self.bus.send(message, timeout=0.2)
    
    def recv(self):
        msg = self.bus.recv()
        if msg is not None:
            # print(msg)
            return msg.arbitration_id, msg.data