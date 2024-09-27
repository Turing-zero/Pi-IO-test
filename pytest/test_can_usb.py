from src.can_py import CAN_PY
import can
import time
import numpy as np

RECV_MODE = False
can_id = [10, 20, 30, 40]

def test_can_recv(can_handle: CAN_PY):
    for i in range(100):
        msg = can_handle.bus.recv()
        if msg is not None:
            print(msg)
        time.sleep(0.001)

    print("stop")

def test_can_send_rpm(can_handle: CAN_PY, id, rpm):
    can_handle.send_rpm(id, rpm)
    # msg = can_handle.bus.recv()
    time.sleep(0.03)

if __name__ == '__main__':
    _can = CAN_PY(_interface='canalystii', _channel=(0), _bitrate=500000)

    if RECV_MODE:
        test_can_recv(_can)
    else:
        for rpm in np.arange(0, 1000, 100):
            for id in can_id:
                test_can_send_rpm(_can, id, rpm)
            time.sleep(0.01)

    del _can