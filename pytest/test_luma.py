import time
from luma.core.interface.serial import spi
from luma.core.render import canvas
from luma.lcd.device import st7789
from PIL import ImageDraw,ImageFont,Image
import RPi.GPIO as GPIO
import smbus2
GPIO.setmode(GPIO.BCM)

bl = 22
rst = 23
GPIO.setup(bl, GPIO.OUT)
GPIO.setup(rst, GPIO.OUT)
GPIO.output(bl,GPIO.HIGH)
GPIO.output(rst,GPIO.HIGH)

# 初始化I2C总线
bus = smbus2.SMBus(1)  # 对于树莓派，通常使用总线1

def read_touch_data():
    try:
        data = bus.read_i2c_block_data(0x15, 0x02, 7)
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
# 初始化 SPI 接口
serial = spi(port=0, device=0, gpio_DC=24, gpio_RST=25, bus_speed_hz=40000000)

# 初始化 ST7789 显示屏
device = st7789(serial, width=320, height=240, rotate=0)

font_path = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"
font_size = 20
font = ImageFont.truetype(font_path, font_size)

# 创建一个小的位图图像
bitmap_image = Image.new("1", (16, 16), 1)  # 创建一个16x16的位图图像
bitmap_draw = ImageDraw.Draw(bitmap_image)
bitmap_draw.rectangle((0, 0, 15, 15), outline="red", fill=0)  # 绘制一个矩形边框
bitmap_draw.line((0, 0, 15, 15), fill=1)  # 对角线1
bitmap_draw.line((0, 15, 15, 0), fill=1)  # 对角线2

# 在显示屏上绘制内容
# while True:
with canvas(device) as draw:
    # device.bounding_box(20,30,200,200)
    # canvas_context = canvas(device)
    # draw = canvas_context.__enter__()
    draw.rectangle((0,0,320,240), outline="white", fill="white")
    draw.rectangle((20,20,280,240), outline="blue", fill="blue")    
    # draw.text((20, 100), "Hello, World!", font=font,fill="green")
    # # 指定线条的起点和终点坐标
    # start_point = (10, 10)  # 起点坐标
    # end_point = (230, 230)  # 终点坐标
    # line_color = "white"    # 线条颜色
    # line_width = 2          # 线条宽度

    # bounding_box = (50, 50, 150, 150)  # 左上角和右下角坐标
    # outline_color = "red"  # 边框颜色
    # fill_color = "green"      # 填充颜色

    # 绘制圆形（实际上是一个宽高相等的椭圆）
    # draw.ellipse(bounding_box, outline=outline_color, fill=None)

    # draw.circle((50,100),60,outline="red")
    # # draw.chord(bounding_box,0,180,outline="red")
    # # draw.arc(bounding_box,0,180,fill="red")
    # draw.pieslice(bounding_box,0,180,outline="red")
    # # 绘制线条
    # draw.line([start_point, end_point], fill=line_color, width=line_width)

    # position = (150, 50)  # 位图的左上角坐标
    # draw.bitmap(position, bitmap_image, fill="green")
    # draw.point((200,100),fill="black")

    # draw.polygon([(50, 150), (150, 150), (100, 200)], outline="red", fill="purple",width=1)
    # draw.regular_polygon(((50,130),30),n_sides=6)
    # draw.rounded_rectangle(((50,50),(100,100)),20,fill="red",corners=(True,True,True,True))


# 保持显示内容一段时间
while True:
    x,y = read_touch_data()
    if x is not None and y is not None:
        print(f"触摸点: ({x}, {y})")
time.sleep(10)
GPIO.output(bl,GPIO.LOW)
