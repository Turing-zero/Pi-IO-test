import busio
import board
import digitalio
import time

data = []
num = 25
spi = busio.SPI(board.SCLK,board.MOSI,board.MISO)

cs = digitalio.DigitalInOut(board.D8)
cs.switch_to_output(value=True)

for i in range(num):
    data.append(i)

spi.try_lock()
spi.configure(baudrate=1000000, phase=0, polarity=0)
cnt = 0
start_time = time.process_time()
while True:
    # cnt = cnt + 1
    spi.write(data)
    cs.value= True
    # time.sleep(0.05)
    cs.value= False
    # end_time = time.process_time()
    # if ((end_time-start_time)*1000 > 1000):
    #     print(cnt)
    #     cnt = 0
    #     start_time = time.process_time()

spi.unlock()
# digitalio.DigitalInOut(board.D25).switch_to_output(value=False)
