from src.can_py import CAN_PY
import can
import time
import threading
import socket
import queue
import binascii
# sudo ip link set can0 up type can bitrate 500000

message_queue = queue.Queue()
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = ('', 12345)
server_socket.bind(server_address)

def udp_receive_thread(stop_event):
    while not stop_event.is_set():
        # 接收消息
        data_raw, address = server_socket.recvfrom(1024)
        # print("recv from {}: {}".format(address, data_raw))
        data_list = [byte for byte in data_raw]
        # print(data_list)
        id = (data_list[0] << 8) + data_list[1]
        data = data_list[2:]
        # print("id: {:x}, data: {}".format(id, data))
        msg = can.Message(arbitration_id=id, is_extended_id=True, data=data)
        # print(msg)
        
        message_queue.put(msg)
        # time.sleep(0.001)

if __name__ == '__main__':
    stop_event = threading.Event()
    receive_thread = threading.Thread(target=udp_receive_thread, args=(stop_event,))
    receive_thread.start()

    _can = CAN_PY()

    for i in range(100000):
        if not message_queue.empty():
            msg = message_queue.get()
            if msg.dlc == 8:
                _can.send_can_msg(msg)
        i = i + 1
        time.sleep(0.001)

    print("stop")
    del _can
    stop_event.set()
    server_socket.close()
    # for i in range(10000):
    #     msg = _can.bus.recv()
    #     if msg is not None:
    #         print(msg)
    #     time.sleep(0.1)

#    # or use an asynchronous notifier
#    notifier = can.Notifier(bus, [can.Logger("recorded.log"), can.Printer()])