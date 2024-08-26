import sys
sys.path.append('..')

from src.can_mcp2515 import CAN_MCP2515
import numpy as np
import time

CAN_ID = 0x123

def pack(header:np.uint8, index:int):
    data = [0, 0, 0, 0, 0, 0, 0, 0]
    data[0] = header
    data[1] = (index >> 8) & 0xff
    data[2] = index & 0xff
    return data

def test_ping_pong(can_handle:CAN_MCP2515, maxcount=1000):
    latency = []
    for i in range(maxcount):
        can_handle.send(CAN_ID, pack(0x0c, 0))
        start_time = time.time()
        id, pong_msg = can_handle.recv()
        end_time = time.time()

        # print(f"Received: {pong_msg}")
        latency.append((end_time - start_time)*1000)
        # print(f"Round trip delay: {latency[:-1]} ms")
    ave = np.mean(latency)
    return ave

def test_hit_rate(can_handle:CAN_MCP2515, freq=1000, maxcount=1000):
    _can.send(CAN_ID, pack(0xab, 0))
    time.sleep(0.01)
    
    for i in range(maxcount):
        can_handle.send(CAN_ID, pack(0x0a, i))
        time.sleep(1.0/freq)
    
    _can.send(CAN_ID, pack(0xba, 0))
    print("hit rate test over!")

if __name__ == '__main__':
    _can = CAN_MCP2515()
    
    test_hit_rate(_can)

    ave = test_ping_pong(_can)
    print("ave: {} ms".format(ave))
