from luma.core.interface.serial import spi
from luma.core.render import canvas
from luma.lcd.device import st7789
from PIL import ImageDraw,ImageFont,Image
import RPi.GPIO as GPIO
import ctypes
import os

class Pair(ctypes.Structure):
    _fields_ = [("first",ctypes.c_double),
                ("second",ctypes.c_double)]

lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__),"../","build", "libdebugger.so"))


class st7789LCD:
    def __init__(self,port,device,dc,lcd_rst,bl,touch_rst,baudrate,width,height,rotate):
        self.serial = spi(port=port, device=device, gpio_DC=dc, gpio_RST=lcd_rst, bus_speed_hz=baudrate)
        self.device = st7789(self.serial, width=width, height=height, rotate=rotate)
        self.font_path = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"
        self.bl = bl
        self.touch_rst = touch_rst
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.bl, GPIO.OUT)
        GPIO.setup(self.touch_rst, GPIO.OUT)
        self.image = Image.new('RGB', (self.device.width, self.device.height))
        self.draw = ImageDraw.Draw(self.image)
         #I2C触控模块初始化
        self.mylib = ctypes.CDLL(lib_path)
        self.mylib.TouchScreen_py.restype = ctypes.c_void_p
        self.mythscreen = self.mylib.TouchScreen_py()

        self.mylib.open_touch_screen_py.argtypes = [ctypes.c_void_p]

        self.mylib.open_touch_screen_py(self.mythscreen)

        self.mylib.get_touch_pos_py.argtypes = [ctypes.c_void_p]
        self.mylib.get_touch_pos_py.restype = Pair


    #打开背光
    def open_screen(self):
        GPIO.output(self.bl,GPIO.HIGH)
    
    #关闭背光
    def close_screen(self):
        GPIO.output(self.bl,GPIO.LOW)

    #将绘图外形的矩形正常化
    def normalLize2p(self,x0,y0,x1,y1):
        norm_xy = ((x0+20,y0),(x1+20,y1))
        return norm_xy
    
    #将绘图的点正常化
    def normalLize(self,x,y):
        norm_xy = (x+20,y)
        return norm_xy
    #绘制矩形
    '''
    xy传入两个坐标,矩形左上角和右下角坐标例如((0,0),(280,280)),fill代表矩形内的填充色
    outline代表矩形边界颜色,width代表矩形绘制的线宽
    '''
    def rectangle(self,x0,y0,x1,y1,fill,outline,width):
        self.draw.rectangle(xy=self.normalLize2p(x0,y0,x1,y1),fill=fill,outline=outline,width=width)
    
    #打印字体
    '''
        参数介绍：
        x,y:需要写入text文本的位置
        data:需要显示的内容
        font_size:字体大小
        fill:字体的颜色 “red”这种直接的颜色或者RGB颜色0xFFFFFF白色
        anchor:"rm" :字体在给定坐标的右边，在坐标中间 "rd" "rt" "md" "mm"  "mt" 
        spacing:行间距
        align:多行文本对齐位置 "left":左对齐  “center”:剧中对齐,“right”:右对齐     
        direction:文本书写方向  “ltr”(默认):从左往右 “rtl”:从右往左 “ttb”:从上到下     只有配置成从上到下才有效果
        features:liga: 标准连字  dlig: 装饰连字  smcp: 小型大写字母 c2sc: 小型大写字母从大写字母转换 frac: 分数 kern: 字距调整 swsh: 花式替换 tnum: 等宽数字 onum: 旧式数字   貌似没什么效果
        language:'en' 英文 ,'fr' 法文 'zh'中文
        stroke_width:文本描边宽度 ,单位为像素
        stroke_fill:文本描边颜色 可以为颜色名字,也可以为RGB颜色
        embedded_color:文本嵌入颜色
    '''
    def text(self,x,y,data,font_size,fill='red',anchor=None,spacing=4,align='left',direction=None,stroke_width=0,stroke_fill=None):
        self.font = ImageFont.truetype(self.font_path, font_size)
        self.draw.text(xy=self.normalLize(x,y),text= data, font=self.font,fill=fill,anchor=anchor,spacing=spacing,align=align,direction=direction,stroke_width=stroke_width,stroke_fill=stroke_fill)

    #绘制椭圆
    '''
    参数介绍：
    (x0,y0)(x1,y1)是绘制椭圆形的外矩形框架的左上角和右下角坐标
    fill:椭圆形内的填充颜色
    outline:椭圆形外边框的颜色 可以传颜色名“red” 也可以传RGB值0xFFFFFF(白色) 
    width:椭圆形外边框的宽度
    '''
    def ellipse(self,x0,y0,x1,y1,fill=None,outline_color=None,width=1):
        self.draw.ellipse(xy=self.normalLize2p(x0,y0,x1,y1), fill=fill, outline=outline_color,width=width)

    #绘制圆形
    '''
    参数介绍：
    (x,y)是圆心
    radius圆半径
    fill是填充颜色
    outline是边框颜色
    width是边框宽度
    '''
    def circle(self,x,y,radius,fill=None,outline=None,width=1):
        self.draw.circle(xy=self.normalLize(x,y),radius=radius,fill=fill,outline=outline,width=width)
    
    #绘制弧线
    '''
    参数介绍：
    (x0,y0)(x1,y1)是绘制弧线椭圆的外矩形框架的左上角和右下角坐标
    start:弧线开始角
    end:弧线终止角
    fill:弧线内的填充颜色
    outline:弧线外边框的颜色 可以传颜色名“red” 也可以传RGB值0xFFFFFF(白色) 
    width:弧线外边框的宽度
    '''
    def chord(self,x0,y0,x1,y1,starta_angle,end_angle,fill=None,outline=None,width=1):
        self.draw.chord(xy=self.normalLize2p(x0,y0,x1,y1),start=starta_angle,end=end_angle,fill=fill,outline=outline,width=width)

    #绘制圆弧
    '''
    参数介绍：
    (x0,y0)(x1,y1)是绘制弧线椭圆的外矩形框架的左上角和右下角坐标
    start:弧线开始角
    end:弧线终止角
    fill:弧线的填充颜色  可以传颜色名“red” 也可以传RGB值0xFFFFFF(白色) 
    width:弧线外边框的宽度
    '''
    def arc(self,x0,y0,x1,y1,start_angle,end_angle,fill=None,width=1):
        self.draw.arc(xy=self.normalLize2p(x0,y0,x1,y1),start=start_angle,end=end_angle,fill=fill,width=width)

    #绘制扇形
    '''
    参数介绍：
    (x0,y0)(x1,y1)是绘制扇形的外矩形框架的左上角和右下角坐标
    start:扇形开始角
    end:扇形终止角
    fill:扇形内的填充颜色
    outline:扇形外边框的颜色 可以传颜色名“red” 也可以传RGB值0xFFFFFF(白色) 
    width:扇形外边框的宽度
    '''
    def pieslice(self,x0,y0,x1,y1,start_angle,end_angle,fill=None,outline=None,width=1):
        self.draw.pieslice(xy=self.normalLize2p(x0,y0,x1,y1),start=start_angle,end=end_angle,fill=fill,outline=outline,width=width)

    # 绘制线条
    '''
    参数介绍：
    (x0,y0)(x1,y1)是连接该线条的两个点
    fill:线的填充颜色
    width:线的宽度
    '''
    def line(self,x0,y0,x1,y1,fill=None,width=1):
        self.draw.line(xy=self.normalLize2p(x0,y0,x1,y1), fill=fill, width=width)

    #绘制小区域图型，
    '''
    参数介绍：
    (x,y)是将该点转化为(0,0)点笛卡坐标系转换
    bitmap是需要绘制的图形组
    fill:线的填充颜色
    '''
    def bitmap(self,x,y,bitmap_image=None,fill=None):
        self.draw.bitmap(xy=self.normalLize(x,y), bitmap=bitmap_image, fill=fill)
    
    #画一个点
    def point(self,x,y,fill=None):
        self.draw.point(self.normalLize(x,y),fill=fill)

    #画多边形
    '''
    参数介绍：
    fill:填充颜色
    outline边框颜色
    width边框宽度
    kwargs是用来填多边形点位的坐标的,例如:x0=50,y0=100,x1=100,y1=200,x2=100,y2=200 后面可以无限添加,但只能添加偶数,因为坐标是双数包括x,y
    '''
    def polygon(self,fill=None,outline=None,width=1,**kwargs):
        points = []
        if len(kwargs)%2 != 0:
            print("please input corret argument")
            return 
        count = 0
        temp = []
        for key,value in kwargs.items():
            count = count+1
            temp.append(value)
            if count % 2 == 0:
                norm_temp = self.normalLize(temp[0],temp[1])
                points.append(norm_temp)
                temp = []
        print(points)
        self.draw.polygon(points, outline=outline, fill=fill,width=width)
    
    #绘制正多边形
    '''
    参数介绍：
    (x,y)是正多边形的圆心
    radius是正多边形半径
    n_sides是多边形有多少条边
    rotation是多变边形需要旋转的角度  单位：度
    fill:填充颜色
    outline边框颜色
    width边框宽度
    '''
    def regular_polygon(self,x,y,radius,n_sides,rotation=0,fill=None,outline=None,width=1):
        self.draw.regular_polygon((self.normalLize(x,y),radius),n_sides=n_sides,rotation=rotation,fill=fill,outline=outline,width=width)
    
    #绘制圆角矩形
    '''
    参数介绍：
    (x0,y0)(x1,y1)是圆角矩形的左上角和右下角
    radius是圆角矩形的圆角半径
    fill是圆角矩形的填充颜色
    outline是边框颜色
    width边框宽度
    cornor里面四个bool值代表矩形的四个角是否需要圆角
    '''
    def rounded_rectangle(self,x0,y0,x1,y1,radius=0,fill=None,outline=None,width=1,corner1=False,corner2=False,corner3=False,corner4=False):
        self.draw.rounded_rectangle(self.normalLize2p(x0,y0,x1,y1),radius=radius,fill=fill,outline=outline,width=width,corners=(corner1,corner2,corner3,corner4))
    
    #读取触摸屏数据
    def read_touch_data(self):
        try:
            data = self.mylib.get_touch_pos_py(self.mythscreen)
                # 解析触摸点数据
            x = data.first
            y = data.second
            return x,y
        except Exception as e:
            print(f"读取触摸数据时出错: {e}")
            return None, None
        
    def display(self):
        self.device.display(self.image)
