import time
from PIL import Image, ImageDraw, ImageFont
import st7789

# 初始化显示屏
disp = st7789.ST7789(
    height=240,
    rotation=90,
    port=0,
    cs=0,  # GPIO 8 (CE0)
    dc=24,
    backlight=19,
    spi_speed_hz=80 * 1000 * 1000
)

disp.begin()

# 创建一个新的图像
width = disp.width
height = disp.height
image = Image.new('RGB', (width, height), color=(0, 0, 0))

# 获取绘图对象以绘制到图像上
draw = ImageDraw.Draw(image)

# 定义字体
font = ImageFont.load_default()

# 绘制一些文本
text = "Hello, World!"
(font_width, font_height) = font.getsize(text)
draw.text(
    (width // 2 - font_width // 2, height // 2 - font_height // 2),
    text,
    font=font,
    fill=(255, 255, 255)
)

# 显示图像
disp.display(image)

# 保持显示
time.sleep(10)