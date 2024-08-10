import smbus2
import time

# I2C地址和寄存器地址
CST816T_I2C_ADDRESS = 0x15
TOUCH_DATA_REG = 0x02

# 初始化I2C总线
bus = smbus2.SMBus(1)  # 对于树莓派，通常使用总线1

def read_touch_data():
    try:
        # 读取寄存器数据
        data = bus.read_i2c_block_data(CST816T_I2C_ADDRESS, TOUCH_DATA_REG, 7)
        print("读取的数据: ", [hex(x) for x in data])  # 调试信息
        if len(data) < 7:
            print("数据长度不正确")
            return None, None
        
        # 解析触摸点数据
        x = data[2];
        y = data[4];
        print(f"x: {x}, y: {y}")
        return x, y
    except Exception as e:
        print(f"读取触摸数据时出错: {e}")
        return None, None


# 主循环，处理触摸输入和显示更新
while True:
    x, y = read_touch_data()
    if x is not None and y is not None:
        print(f"触摸点: ({x}, {y})")
    time.sleep(0.5)