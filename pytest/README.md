# pytest

## test_can_performance

This script needs to be run together with another device connected to the CAN bus interface. The specific code running on other devices depends on the type of CAN device. The following provides a general logic.

```python
if __name__ == '__main__':
    index_start = 0
    index_end = 0
    recv_count = 0
    while True:
        id, data = _can.receive()
        if data is not None:
            # print("id: {:x}, data: {}".format(id, data))
            if data[0] == 0xab:
                # hit rate test start
                index = (data[1] << 8) + data[2]
                index_start = index
                recv_count = 0
            if data[0] == 0x0a:
                # hit rate test process
                index = (data[1] << 8) + data[2]
                index_end = index + 1
                recv_count += 1
            if data[0] == 0xba:
                # hit rate test end
                rate = recv_count / (index_end - index_start)
                print("recv: {}, index_start: {}, index_end: {}, rate: {}".format(recv_count, index_start, index_end, rate))
            if data[0] == 0x0c:
                # pingpong test
                _can.send(0x100, [0, 0, 0, 0, 0, 0, 0, 0])
```

## test_can_udp

Receive 9 bytes message(1st: **ID**, 2nd-9th: 8bytes **DATA**) through UDP, and transfer 8bytes **DATA** to the corresponding CAN **ID**.