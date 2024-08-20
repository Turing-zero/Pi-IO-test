import st7789_lcd 
import time
from PIL import Image,ImageDraw

# lcd = lcd_lib.LCD(0,board.D24,0,board.D9,50000000,280,240,20,90)

lcd = st7789_lcd.st7789LCD(0,0,24,25,22,23,40000000,320,240,0)


def read_touch_data():
    start_time = time.process_time()
    count = 0
    while True:
        # x,y = lcd.get_touch_pos()
        # print(f"x: {x}, y: {y}")
        # print("xxx")
        count = count+1
        end_time = time.process_time()
        if ((end_time-start_time)*1000 > 1000):
            print(count)
            count = 0
            start_time = time.process_time()


# 创建一个小的位图图像
bitmap_image = Image.new("1", (16, 16), 1)  # 创建一个16x16的位图图像
bitmap_draw = ImageDraw.Draw(bitmap_image)
bitmap_draw.rectangle((0, 0, 15, 15), outline="red", fill=0)  # 绘制一个矩形边框
bitmap_draw.line((0, 0, 15, 15), fill=1)  # 对角线1
bitmap_draw.line((0, 15, 15, 0), fill=1)  # 对角线2

lcd.open_screen()
while True:
    lcd.rectangle(x0=0,y0=0,x1=279,y1=239,fill="red",outline="blue",width=1)
    lcd.rectangle(x0=20,y0=20,x1=259,y1=219,fill="white",outline="blue",width=1)
    lcd.text(x=140,y=120,data="Hellow world",font_size=24,fill='green',align="center",direction="ltr",stroke_width=4,stroke_fill='red',anchor="mt")
    lcd.ellipse(x0=0,y0=0,x1=50,y1=100,fill='blue',outline_color='red',width=3)
    lcd.circle(x=100,y=200,radius=60,fill=None,outline="blue",width=1)
    lcd.chord(x0=50,y0=50,x1=150,y1=150,starta_angle=0,end_angle=180,fill=None,outline="green",width=10)
    lcd.arc(x0=100,y0=50,x1=200,y1=200,start_angle=0,end_angle=180,fill="blue",width=2)
    lcd.pieslice(x0=100,y0=150,x1=200,y1=200,start_angle=0,end_angle=180,fill=None,outline="blue",width=2)
    lcd.bitmap(x=150,y=50,bitmap_image=bitmap_image,fill="green")
    lcd.point(x=200,y=60,fill="black")
    lcd.regular_polygon(x=150,y=100,radius=60,n_sides=3,rotation=180,fill="red")
    lcd.rounded_rectangle(x0=100,y0=100,x1=150,y1=150,radius=20,fill="purple",outline=None,corner1=True,corner2=True,corner3=True,corner4=True)
    lcd.polygon(fill="red",outline="purple",width=1,x0=50,y0=150,x1=150,y1=150,x2=100,y2=200)
    lcd.display()

time.sleep(10)