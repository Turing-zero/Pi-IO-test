import time
import ctypes
import os
import smbus2

# I2C地址和寄存器地址
CST816T_I2C_ADDRESS = ctypes.c_uint8(0x15)
TOUCH_DATA_REG = ctypes.c_uint8(0x02)

lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__),"../","build", "libdebugger.so"))

mylib = ctypes.CDLL(lib_path)

class Pair(ctypes.Structure):
    _fields_ = [("first",ctypes.c_double),
                ("second",ctypes.c_double)]

    
mylib.TouchScreen_py.restype = ctypes.c_void_p
mythscreen = mylib.TouchScreen_py()

mylib.open_touch_screen_py.argtypes = [ctypes.c_void_p]

mylib.open_touch_screen_py(mythscreen)

mylib.get_touch_pos_py.argtypes = [ctypes.c_void_p]
mylib.get_touch_pos_py.restype = Pair

# 初始化I2C总线
bus = smbus2.SMBus(1)  # 对于树莓派，通常使用总线1

def read_touch_data():
    try:
        # 读取寄存器数据

        data = mylib.get_touch_pos_py(mythscreen)

        # 解析触摸点数据
        x = data.first;
        y = data.second;
        return x, y
    except Exception as e:
        print(f"读取触摸数据时出错: {e}")
        return None, None

# 主循环，处理触摸输入和显示更新
while True:
    x, y = read_touch_data()
    if x is not None and y is not None:
        print(f"触摸点: ({x}, {y})")
    pass
