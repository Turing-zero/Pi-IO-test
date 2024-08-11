import board
import busio
import terminalio
import displayio
import smbus2
import time
try:
    from fourwire import FourWire
except ImportError:
    from displayio import FourWire
from adafruit_display_text import label
from adafruit_st7789 import ST7789

#SPI1 引脚  board.Dx x是树莓派上的GPIOx
SPI1_SCLK = board.D21
SPI1_MOSI = board.D20
SPI1_MISO = board.D19
SPI1_CE0  = board.D18
SPI1_CE1  = board.D17
SPI1_CE2  = board.D16

# CST816T I2C地址
CST816T_I2C_ADDRESS = 0x15
TOUCH_DATA_REG = 0x02


class LCD:
    def __init__(self,Spi_num,Dc,Ce_num,Reset,Baudrate,Width,Height,Rowstart,Rotation):#spi默认是spi0,要配置spi1需要手动配置
        displayio.release_displays()
        #SPI  显示模块
        if Spi_num==1:  
            self.spi = busio.SPI(SPI1_SCLK,SPI1_MOSI,SPI1_MISO)
            if Ce_num == 0:
                self.tft_cs = SPI1_CE0
            elif  Ce_num == 1:
                self.tft_cs = SPI1_CE1
            elif Ce_num == 2:
                self.tft_cs = SPI1_CE2
            else:
                print("Please Select CE0~CE2 for SPI1")
                return 
        else:
            self.spi = busio.SPI(board.SCLK,board.MOSI,board.MISO)
            if Ce_num == 0:
                self.tft_cs = board.CE0
            elif  Ce_num == 1:
                self.tft_cs = board.CE1
            else:
                print("Please Select CE0~CE1 for SPI0")
                return 
        self.display_bus = FourWire(self.spi, command=Dc, chip_select=self.tft_cs, reset=Reset,baudrate=Baudrate)#SPI初始化
        self.display = ST7789(self.display_bus, width=Width, height=Height, rowstart=Rowstart, rotation=Rotation)#显示模块初始化
        self.splash = displayio.Group() #用于管理一个显示对象
        self.display.root_group = self.splash #用于根显示组

        #I2C触控模块初始化
        self.bus = smbus2.SMBus(1)

        #图像存储
        self.rectangle = []
        self.text = []

    
    #画一个基于x，y当一个顶点的矩形,传入参数 宽度、高度、深度、坐标
    def draw_rectangle(self,width,height,depth,X,Y,color):
        color_bitmap = displayio.Bitmap(width,height,depth) #
        color_palette = displayio.Palette(depth)
        color_palette[0] = color

        bg_sprite = displayio.TileGrid(color_bitmap, pixel_shader=color_palette, x=X, y=Y)#绘画
        self.rectangle.append(bg_sprite)
        self.splash.append(self.rectangle[0])

    #删除已经画过的矩形
    def remove_rectangle(self,index):
        print(index)
        self.splash.remove(self.rectangle[index])
        self.rectangle.remove(self.rectangle[index])

    def draw_label(self,Scale,X,Y,text,color):
        text_group = displayio.Group(scale=Scale,x=X,y=Y)
        text = text
        text_area = label.Label(terminalio.FONT,text=text,color=color)
        text_group.append(text_area)
        self.text.append(text_group)
        self.splash.append(text_group)

    #移除画的字体
    def remove_label(self,index):
        self.splash.remove(self.text[index])
        self.text.remove(self.text[index])


    def get_touch_pos(self):
        try:
            data = self.bus.read_i2c_block_data(CST816T_I2C_ADDRESS, 0x00, 7)
            print("读取的数据: ", [hex(x) for x in data])  # 调试信息
            if len(data) < 7:
                print("数据长度不正确")
                return None, None
            
            # 解析触摸点数据
            x = data[4];
            y = data[6];
            print(f"x: {x}, y: {y}")
            return x, y
        except Exception as e:
            print(f"读取触摸数据时出错: {e}")
            return None, None





            