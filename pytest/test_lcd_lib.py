import lcd_lib
import board 

lcd = lcd_lib.LCD(0,board.D24,0,board.D9,24000000,280,240,20,90)

lcd.draw_rectangle(280,240,1,0,0,0xAA0088)
lcd.draw_label(3,37,120,"Hellow RPI!",0xFFFF00)

count = 0
while True:
    count=count+1
    # if count==1000000:
    #     lcd.remove_rectangle(0)
    pass