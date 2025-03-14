# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

"""
This test will initialize the display using displayio and draw a solid green
background, a smaller purple rectangle, and some yellow text.
"""
import board
import busio
import terminalio
import displayio
import smbus2
import time
import cProfile

# Starting in CircuitPython 9.x fourwire will be a seperate internal library
# rather than a component of the displayio library
try:
    from fourwire import FourWire
except ImportError:
    from displayio import FourWire
from adafruit_display_text import label
from adafruit_st7789 import ST7789
# Release any resources currently in use for the displays
displayio.release_displays()

#默认是SPI0 如果要配置SP1的话需要手动配置  例如board.D35代表spi1 MISO 是物理引脚35
# spi = board.SPI()
spi = busio.SPI(board.SCLK,board.MOSI,board.MISO)

# CST816T I2C地址
CST816T_I2C_ADDRESS = 0x15
TOUCH_DATA_REG = 0x02

# 初始化I2C总线
bus = smbus2.SMBus(1)  # 对于树莓派，通常使用总线1

def read_touch_data():
    try:
        data = bus.read_i2c_block_data(CST816T_I2C_ADDRESS, 0x02, 7)
        # print("读取的数据: ", [hex(x) for x in data])  # 调试信息
        if len(data) < 7:
            print("数据长度不正确")
            return None, None
        
        # 解析触摸点数据
        x = data[2];
        y = data[4];
        # print(f"x: {x}, y: {y}")
        return x, y
    except Exception as e:
        print(f"读取触摸数据时出错: {e}")
        return None, None
tft_cs = board.CE0
tft_dc = board.D24

display_bus = FourWire(spi, command=tft_dc, chip_select=tft_cs, reset=board.D25,baudrate=8000000)#SPI初始化

display = ST7789(display_bus, width=280, height=240, rowstart=20, rotation=90)

# Make the display context
splash = displayio.Group() #用于管理一个显示对象
display.root_group = splash #用于根显示组

color_bitmap = displayio.Bitmap(280, 240, 1)#第一个参数是图像宽度，第二个参数是图像高度，第三个参数是图像深度
color_palette = displayio.Palette(1)#图像颜色数量    调色板
color_palette[0] = 0x00FF00  # Bright Green

bg_sprite = displayio.TileGrid(color_bitmap, pixel_shader=color_palette, x=0, y=0)#
splash.append(bg_sprite)

# Draw a smaller inner rectangle
inner_bitmap = displayio.Bitmap(240, 200, 1)
inner_palette = displayio.Palette(1)
inner_palette[0] = 0xAA0088  # Purple
inner_sprite = displayio.TileGrid(inner_bitmap, pixel_shader=inner_palette, x=20, y=20)
splash.append(inner_sprite)

# Draw a label
text_group = displayio.Group(scale=3, x=37, y=120)
text = "Hello RPI!"
text_area = label.Label(terminalio.FONT, text=text, color=0xFFFF00)
text_group.append(text_area)  # Subgroup for text scaling
splash.append(text_group)

# display.show(None)
# def touch_thread():

# print(f"Import specific module time: {(end_time - start_time)*1000} ms")
def main():
    count = 0
    start_time = time.process_time()
    while True:
        count = count + 1
        # x, y = read_touch_data()
        # if x is not None and y is not None:
        #     print(f"触摸点: ({x}, {y})")
        # time.sleep(0.5)
        end_time = time.process_time()
        # if ((end_time-start_time)*1000 > 1000):
        #     print(count)
        #     count = 0
        #     start_time = time.process_time()

if __name__ == "__main__":
    # cProfile.run('main()')
    main()


# while True:
#     pass