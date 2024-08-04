import spidev

# 打开 SPI 设备
spi = spidev.SpiDev()
spi.open(0, 0)  # 打开 SPI 总线 0, 设备 0

# 查看当前速率
current_speed_hz = spi.max_speed_hz
print("Current SPI speed: {} Hz".format(current_speed_hz))

# 设置新的速率（例如 500 kHz）
# spi.max_speed_hz = 500000
# print("New SPI speed: {} Hz".format(spi.max_speed_hz))

# 关闭 SPI 设备
spi.close()
