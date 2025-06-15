from can_vesc import VESC
import time

# from can_interfaces.usb_canfd import *
# if __name__ == "__main__":
    # channels = [0, 1]
    # vesc_handle_1 = VESC(CANUsbCanfd(*channels))
    
    # can_id = [30, 31, 32, 33, 34, 35]
    # # Sending a message
    # vesc_handle_1.send_rpm(can_id[0], 0.5)
    # time.sleep(0.1)
    
    # # Receiving a message
    # id, data = vesc_handle_1.receive_decode(timeout=-1)
    # if data is not None:
    #     print("RECV vesc id: {}, data: {}".format(id & 0xff, data))
    # else:
    #     print("No data received")

from can_interfaces.usb_wit import *
if __name__ == "__main__":
    can_handle = CANUsbWit('/dev/ttyUSB0', 0.01, 500000)
    vesc_handle_2 = VESC(can_handle)
    # mode: SEND
    # for i in range(100):
    #     vesc_handle_2.send_rpm(30, i)
    #     time.sleep(0.001)
    # mode: RECV
    try:
        while True:
            id, data = vesc_handle_2.receive_decode(timeout=0.01)
            if id is not None:
                print("RECV vesc id: {}, rpm: {}".format(id & 0xff, data.rpm))
            # time.sleep(0.01)
    except KeyboardInterrupt:
        print("KeyboardInterrupt received, exiting...")
    finally:
        can_handle.close()
        print("CAN interface closed.")