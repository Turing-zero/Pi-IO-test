from ctypes import *
from .base import CANInterfaceBase
import numpy as np
import can
import os

script_dir = os.path.dirname(os.path.abspath(__file__))

VCI_USBCAN2 = 4
VCI_USBCANFD = 41
STATUS_OK = 1
INVALID_DEVICE_HANDLE = 0
INVALID_CHANNEL_HANDLE = 0

class VCI_INIT_CONFIG(Structure):
    _fields_ = [("AccCode", c_uint),
                ("AccMask", c_uint),
                ("Reserved", c_uint),
                ("Filter", c_ubyte),
                ("Timing0", c_ubyte),
                ("Timing1", c_ubyte),
                ("Mode", c_ubyte)
                ]  
    
class VCI_CAN_OBJ(Structure):
    _fields_ = [("ID", c_uint),
                ("TimeStamp", c_uint),
                ("TimeFlag", c_ubyte),
                ("SendType", c_ubyte),
                ("RemoteFlag", c_ubyte),
                ("ExternFlag", c_ubyte),
                ("DataLen", c_ubyte),
                ("Data", c_ubyte*8),
                ("Reserved", c_ubyte*3)
                ]

# structure
class _ZCAN_CHANNEL_CAN_INIT_CONFIG(Structure):
    _fields_ = [("acc_code", c_uint),
                ("acc_mask", c_uint),
                ("reserved", c_uint),
                ("filter",   c_ubyte),
                ("timing0",  c_ubyte),
                ("timing1",  c_ubyte),
                ("mode",     c_ubyte)]

class _ZCAN_CHANNEL_CANFD_INIT_CONFIG(Structure):
    _fields_ = [("acc_code",     c_uint),
                ("acc_mask",     c_uint),
                ("abit_timing",  c_uint),
                ("dbit_timing",  c_uint),
                ("brp",          c_uint),
                ("filter",       c_ubyte),
                ("mode",         c_ubyte),
                ("pad",          c_ushort),
                ("reserved",     c_uint)]

class _ZCAN_CHANNEL_INIT_CONFIG(Union):
    _fields_ = [("can", _ZCAN_CHANNEL_CAN_INIT_CONFIG),
                ("canfd", _ZCAN_CHANNEL_CANFD_INIT_CONFIG)]

class ZCAN_CHANNEL_INIT_CONFIG(Structure):
    _fields_ = [("can_type", c_uint),
                ("config", _ZCAN_CHANNEL_INIT_CONFIG)]

class ZCAN_CAN_FRAME(Structure):
    _fields_ = [("can_id",  c_uint, 29),
                ("err",     c_uint, 1),
                ("rtr",     c_uint, 1),
                ("eff",     c_uint, 1),
                ("can_dlc", c_ubyte),
                ("__pad",   c_ubyte),
                ("__res0",  c_ubyte),
                ("__res1",  c_ubyte),
                ("data",    c_ubyte * 8)]

class ZCAN_CANFD_FRAME(Structure):
    _fields_ = [("can_id", c_uint, 29),
                ("err",    c_uint, 1),
                ("rtr",    c_uint, 1),
                ("eff",    c_uint, 1),
                ("len",    c_ubyte),
                ("brs",    c_ubyte, 1),
                ("esi",    c_ubyte, 1),
                ("__res",  c_ubyte, 6),
                ("__res0", c_ubyte),
                ("__res1", c_ubyte),
                ("data",   c_ubyte * 64)]

class ZCAN_Transmit_Data(Structure):
    _fields_ = [("frame", ZCAN_CAN_FRAME), ("transmit_type", c_uint)]

class ZCAN_Receive_Data(Structure):
    _fields_ = [("frame", ZCAN_CAN_FRAME), ("timestamp", c_ulonglong)]

class ZCAN_TransmitFD_Data(Structure):
    _fields_ = [("frame", ZCAN_CANFD_FRAME), ("transmit_type", c_uint)]

class ZCAN_ReceiveFD_Data(Structure):
    _fields_ = [("frame", ZCAN_CANFD_FRAME), ("timestamp", c_ulonglong)]

class USBCANFD:
    def __init__(self, lib_path: str):
        self.canDLL = cdll.LoadLibrary(lib_path)

        self.canDLL.ZCAN_OpenDevice.restype = c_void_p
        self.canDLL.ZCAN_SetAbitBaud.argtypes = (c_void_p, c_ulong, c_ulong)
        self.canDLL.ZCAN_SetDbitBaud.argtypes = (c_void_p, c_ulong, c_ulong)
        self.canDLL.ZCAN_SetCANFDStandard.argtypes = (c_void_p, c_ulong, c_ulong)
        self.canDLL.ZCAN_InitCAN.argtypes = (c_void_p, c_ulong, c_void_p)
        self.canDLL.ZCAN_InitCAN.restype = c_void_p
        self.canDLL.ZCAN_StartCAN.argtypes = (c_void_p,)
        self.canDLL.ZCAN_Transmit.argtypes = (c_void_p, c_void_p, c_ulong)
        self.canDLL.ZCAN_TransmitFD.argtypes = (c_void_p, c_void_p, c_ulong)
        self.canDLL.ZCAN_GetReceiveNum.argtypes = (c_void_p, c_ulong)
        self.canDLL.ZCAN_Receive.argtypes = (c_void_p, c_void_p, c_ulong, c_long)
        self.canDLL.ZCAN_ReceiveFD.argtypes = (c_void_p, c_void_p, c_ulong, c_long)
        self.canDLL.ZCAN_ResetCAN.argtypes = (c_void_p,)
        self.canDLL.ZCAN_CloseDevice.argtypes = (c_void_p,)
        self.canDLL.ZCAN_ClearFilter.argtypes=(c_void_p,)
        self.canDLL.ZCAN_AckFilter.argtypes=(c_void_p,)
        self.canDLL.ZCAN_SetFilterMode.argtypes=(c_void_p,c_ulong)
        self.canDLL.ZCAN_SetFilterStartID.argtypes=(c_void_p,c_ulong)
        self.canDLL.ZCAN_SetFilterEndID.argtypes=(c_void_p,c_ulong)

        self.can_device = self.canDLL.ZCAN_OpenDevice(VCI_USBCANFD, 0, 0)
        # if self.can_device == INVALID_DEVICE_HANDLE:
        #     print("Open Device failed!")
        #     exit(0)
        # print("Open Device OK, device handle:0x%x." %(self.can_device))
        self.can_channels = [INVALID_CHANNEL_HANDLE, INVALID_CHANNEL_HANDLE]

    def __del__(self):
        ret = self.canDLL.ZCAN_CloseDevice(self.can_device)
        if ret != STATUS_OK:
            print("Close Device failed!")
            # exit(0)
        print("Close Device OK!")

    def reconfigure_channel(self, channel: int, abitrate: int, dbitrate: int, fd_on: bool):
        self.canDLL.ZCAN_ResetCAN(self.can_channels[channel])
        self.start_channel(channel, abitrate, dbitrate, fd_on)

    def start_channel(self, channel: int = 0, abitrate: int = 500000, dbitrate: int = 500000, fd_on: bool = False):
        self.canDLL.ZCAN_SetAbitBaud(self.can_device, channel, abitrate)
        self.canDLL.ZCAN_SetDbitBaud(self.can_device, channel, dbitrate)
        self.canDLL.ZCAN_SetCANFDStandard(self.can_device, channel, 0)

        self.zcan_initconfig = ZCAN_CHANNEL_INIT_CONFIG()
        self.zcan_initconfig.can_type = 1  # 0 for CAN, 1 for CANFD
        self.zcan_initconfig.config.canfd.mode = 0
        can_ch_handler = self.canDLL.ZCAN_InitCAN(self.can_device, channel, byref(self.zcan_initconfig))
        # 启动通道0
        ret = self.canDLL.ZCAN_StartCAN(can_ch_handler)
        if ret != STATUS_OK:
            print("Start CAN #{} failed!".format(channel))
            exit(0)
        print("Start CAN #{} OK!".format(channel))

        if channel < 2:
            self.can_channels[channel] = can_ch_handler
        else:
            print("Channel index out of range, only 0 and 1 are supported.")
            exit(0)
        # if self.dev_ch1 == INVALID_CHANNEL_HANDLE:
        #     print("Init CAN0 failed!")
        #     exit(0)
        # print("Init CAN0 OK!")

    def send_one(self, _id, _data, channel: int = 0):
        if len(_data) > 8:
            print('Data length should be less than 8')
            _data = _data[:8]
        can_msgs = (ZCAN_Transmit_Data * 1)()
        can_msgs[0].transmit_type = 0
        can_msgs[0].frame.eff = 1  # extended frame
        can_msgs[0].frame.rtr = 0  # data frame
        can_msgs[0].frame.can_id = _id
        can_msgs[0].frame.can_dlc = len(_data)

        for i in range(len(_data)):
            can_msgs[0].frame.data[i] = _data[i]

        ret = self.canDLL.ZCAN_Transmit(self.can_channels[channel], byref(can_msgs), 1)
        return ret

    def send_multiple(self, _id, _data, repeat: int = 1, channel: int = 0):
        if len(_data) > 8:
            print('Data length should be less than 8')
            _data = _data[:8]
        can_msgs = (ZCAN_Transmit_Data * repeat)()
        for i in range(repeat):
            can_msgs[i].transmit_type = 0
            can_msgs[i].frame.can_id = _id
            can_msgs[i].frame.can_dlc = len(_data)
            can_msgs[i].frame.eff = 1
            can_msgs[i].frame.rtr = 0  # data frame
            for j in range(len(_data)):
                can_msgs[i].frame.data[j] = _data[j]
        ret = self.canDLL.ZCAN_Transmit(self.can_channels[channel], byref(can_msgs), repeat)
        if ret > 0:
            print("Sent {} messages on channel {}".format(ret, channel))
        else:
            print("Failed to send messages on channel {}".format(channel))
        return ret

    # TODO: FD
    def receive_one(self, channel: int = 0, timeout: int = -1) -> tuple[int, list]:
        num = self.canDLL.ZCAN_GetReceiveNum(self.can_channels[channel], 0)
        if num > 0:
            can_msgs = (ZCAN_Receive_Data * num)()
            ret = self.canDLL.ZCAN_Receive(self.can_channels[channel], byref(can_msgs), num, timeout)
            if ret > 0:
                # print("Received {} messages on channel {}".format(ret, channel))
                return can_msgs[0].frame.can_id, list(can_msgs[0].frame.data[:can_msgs[0].frame.can_dlc])
            else:
                return None, None
        else:
            # print("No messages received on channel {}".format(channel))
            return None, None

    def get_dll(self):
        return self.canDLL

class CANUsbCanfd(CANInterfaceBase):
    """
    USB CAN FD interface initialization.

    :param channels: List of channels to initialize, default is [0].

    :param abaudrate: Arbitration bitrate, default is 500000.

    :param dbitrate: Data bitrate, default is 500000.
    
    :param fd_on: Enable CAN FD mode, default is False.
    """
    def __init__(self, *channels, abaudrate=500000, dbitrate=500000, fd_on=False):
        # default channel: 0
        if not channels:
            channels = [0]
        self.channels = channels
        
        file_path = os.path.join(script_dir, 'libs', 'ControlCANFD.dll')
        self.can_handler = USBCANFD(file_path)
        # self.can_handler = USBCANFD('./libs/libcontrolcanfd.so')
        for channel in channels:
            self.can_handler.start_channel(channel, abaudrate, dbitrate, fd_on)

    def __del__(self):
        del self.can_handler

    def reconfigure(self, channel, abitrate, dbitrate, fd_on):
        self.can_handler.reconfigure_channel(channel, abitrate, dbitrate, fd_on)

    def send(self, _id, _data, can_channel=0):
        ret = self.can_handler.send_one(_id, _data, channel=can_channel)
        # ret = self.can_handler.send_multiple(_id, _data, repeat=10, channel=can_channel)
        # print("\r\n CAN0 Tranmit CAN Num: %d." % ret)
    
    def receive(self, timeout, can_channel=0) -> tuple[np.uint32, list]:
        _id, _data = self.can_handler.receive_one(channel=can_channel, timeout=timeout)
        if (_id is not None) and (_data is not None):
            # TODO: multi messages
            return _id, _data
        else:
            return None, None