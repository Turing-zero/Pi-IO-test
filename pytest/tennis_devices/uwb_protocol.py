import struct
import json
from typing import Union, Dict, List

class DecodeError(Exception):
    """自定义解码异常"""
    pass

def decode_TWR_USB(data: bytes) -> Dict[str, Union[int, float, list]]:
    """
    解码TWR-USB协议（HEX格式）
    协议结构：head(6B) + len(1B) + Timer(4B) + ... + Check(1B) + Foot(1B)
    """
    if len(data) == data[7]+4:  # 最小长度校验
        return None
        raise DecodeError("Invalid data length for TWR-USB")

    # 1. 校验帧头和帧尾
    if data[0:6] != b'CmdM:4':
        return None
        raise DecodeError("Invalid header for TWR-USB")
    if data[-2:] != b'\r\n':  # Foot 应为\n
        return None
        raise DecodeError("Invalid footer for TWR-USB")

    # 2. 校验和验证 (XOR校验，范围从head到Pos_TagZ)
    calc_checksum = 0
    for byte in data[6:-4]:  # 排除最后两个字节(Check+Foot)
        calc_checksum ^= byte
    if calc_checksum != data[-3]:
        return None
        raise DecodeError("Checksum mismatch in TWR-USB")

    # 3. 解析字段（小端模式）
    pos = 6  # 跳过head
    data_len = data[pos]; pos += 1
    timer = struct.unpack('<I', data[pos:pos+4])[0]; pos += 4
    tag_id = struct.unpack('<h', data[pos:pos+2]); pos += 2  # 16位Tag ID
    anc_id = struct.unpack('<h', data[pos:pos+2]); pos += 2  # 16位ANC ID
    seq = data[pos]; pos += 1
    mask = data[pos]; pos += 1
    
    # 原始测距数据 (8个基站 * 4字节)
    raw_ranges = []
    for _ in range(8):
        raw_ranges.append(struct.unpack('<I', data[pos:pos+4])[0])
        pos += 4
    
    kalman_enable = data[pos]; pos += 1
    
    # 卡尔曼滤波数据 (8个基站 * 4字节)
    kalman_ranges = []
    for _ in range(8):
        kalman_ranges.append(struct.unpack('<I', data[pos:pos+4])[0])
        pos += 4
    
    pos_enable = data[pos]; pos += 1
    pos_dimen = data[pos]; pos += 1
    pos_anc_mask = data[pos]; pos += 1
    pos_tag_res = data[pos]; pos += 1
    pos_x = struct.unpack('<I', data[pos:pos+4])[0]; pos += 4
    pos_y = struct.unpack('<I', data[pos:pos+4])[0]; pos += 4
    pos_z = struct.unpack('<I', data[pos:pos+4])[0]; pos += 4

    return {
        'protocol': 'TWR-USB',
        'timer': timer,
        'tag_id': tag_id,
        'anc_id': anc_id,
        'seq': seq,
        'mask': mask,
        'raw_ranges': raw_ranges,
        'kalman_enable': kalman_enable,
        'kalman_ranges': kalman_ranges,
        'pos_enable': pos_enable,
        'pos_dimen': pos_dimen,
        'pos_anc_mask': pos_anc_mask,
        'pos_tag_res': pos_tag_res,
        'pos_x': pos_x,
        'pos_y': pos_y,
        'pos_z': pos_z
    }

def decode_TWR_UART2(data: bytes) -> Dict[str, Union[int, list]]:
    """
    解码TWR-UART2协议
    协议结构：帧头(1B) + 长度(1B) + 版本(1B) + 数据域(32B) + 校验和(1B) + 帧尾(1B)
    """
    # if len(data) != 37:  # 固定长度
    #     raise DecodeError("Invalid data length for TWR-UART2")

    # 1. 校验帧头和帧尾
    if data[0] != 0xAA:
        raise DecodeError("Invalid header for TWR-UART2")
    if data[-1] != 0x55:
        raise DecodeError("Invalid footer for TWR-UART2")
    
    # 2. 长度校验 (长度字段值应为35)
    length = data[1]
    if length != len(data):
        print(data)
        print(f"Expected length: {len(data)}, got: {length}")
        raise DecodeError("Length field mismatch in TWR-UART2")
    
    # 3. 校验和验证 (累加和取余256)
    calc_checksum = sum(data[0:35]) % 256  # 帧头到校验和前
    if calc_checksum != data[35]:
        raise DecodeError("Checksum mismatch in TWR-UART2")
    
    # 4. 解析数据域 (8个基站 * 4字节)
    ranges = []
    pos = 3  # 跳过帧头+长度+版本
    for _ in range(8):
        ranges.append(struct.unpack('<I', data[pos:pos+4])[0])  # mm
        pos += 4

    return {
        'protocol': 'TWR-UART2',
        'version': data[2],
        'kalman_ranges': ranges  # 基站0~7的测距数据
    }

def decode_PDOA_HEX(data: bytes) -> Dict[str, Union[int, float]]:
    """
    解码PDOA-HEX协议
    协议结构：head(1B) + en(1B) + sn(1B) + ... + Check(1B) + Foot(1B)
    """
    if len(data) < 31:  # 最小长度校验
        raise DecodeError("Invalid data length for PDOA-HEX")

    # 1. 校验帧头和帧尾
    if data[0] != 0x2A:
        raise DecodeError("Invalid header for PDOA-HEX")
    if data[-1] != 0x23:
        raise DecodeError("Invalid footer for PDOA-HEX")
    
    # 2. 长度校验
    en = data[1]  # sn到Acc_Z的长度
    if len(data) != 3 + en + 2:  # head+en + (sn~Acc_Z) + Check+Foot
        raise DecodeError("Length field mismatch in PDOA-HEX")
    
    # 3. 校验和验证 (XOR校验)
    calc_checksum = 0
    for byte in data[0:-2]:  # 排除最后两个字节(Check+Foot)
        calc_checksum ^= byte
    if calc_checksum != data[-2]:
        raise DecodeError("Checksum mismatch in PDOA-HEX")

    # 4. 解析字段（小端模式）
    pos = 2  # 跳过head+en
    sn = data[pos]; pos += 1
    addr = struct.unpack('<H', data[pos:pos+2])[0]; pos += 2
    angle = struct.unpack('<I', data[pos:pos+4])[0]; pos += 4
    distance = struct.unpack('<I', data[pos:pos+4])[0]; pos += 4
    user_cmd = struct.unpack('<H', data[pos:pos+2])[0]; pos += 2
    f_path_power = struct.unpack('<I', data[pos:pos+4])[0]; pos += 4
    rx_level = struct.unpack('<I', data[pos:pos+4])[0]; pos += 4
    acc_x = struct.unpack('<h', data[pos:pos+2])[0]; pos += 2
    acc_y = struct.unpack('<h', data[pos:pos+2])[0]; pos += 2
    acc_z = struct.unpack('<h', data[pos:pos+2])[0]

    # 解析user_cmd位域
    is_lowbattery = bool(user_cmd & 0x01)
    is_alarm = bool(user_cmd & 0x02)

    return {
        'protocol': 'PDOA-HEX',
        'sn': sn,
        'addr': addr,
        'angle': angle,
        'distance': distance,
        'is_lowbattery': is_lowbattery,
        'is_alarm': is_alarm,
        'f_path_power': f_path_power,
        'rx_level': rx_level,
        'acc_x': acc_x,
        'acc_y': acc_y,
        'acc_z': acc_z
    }

def decode_data(data: bytes) -> Dict:
    """自动识别并解码三种协议格式"""
    if not data:
        return None
        # raise DecodeError("Empty data received")
    
    # 根据第一个字节识别协议
    first_byte = data[0]
    last_byte = data[-1]
    
    if first_byte == 0xAA and last_byte == 0x55:
        return decode_TWR_UART2(data)
    elif len(data) >= 6 and data[0:6] == b'CmdM:4':
        return decode_TWR_USB(data)
    elif first_byte == 0x2A and last_byte == 0x23:
        return decode_PDOA_HEX(data)
    
    # 尝试解析JSON格式的PDOA
    try:
        return {'protocol': 'PDOA-JSON', 'data': json.loads(data.decode())}
    except:
        # raise DecodeError("Unrecognized protocol format")
        return None