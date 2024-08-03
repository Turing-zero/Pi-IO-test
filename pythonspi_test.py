import spidev
import time

# 初始化 SPI
spi = spidev.SpiDev()
spi.open(0, 0)  # 打开 SPI 设备0, 从设备0
spi.max_speed_hz = 8000000  # 设置 SPI 速度为1 MHz

num_samples = 1000
timestamps = []

# 记录每次采样的时间戳
for _ in range(num_samples):
    start_time = time.time()
    spi.xfer2([0xF1, 0x20])  # 模拟读取 ADC 数据
    end_time = time.time()
    timestamps.append(end_time - start_time)

# 计算每次采样的时间间隔
elapsed_times = [timestamps[i] for i in range(len(timestamps))]
average_elapsed_time = sum(elapsed_times) / len(elapsed_times)

# 计算平均采样频率
average_sample_rate = 1 / average_elapsed_time

print(f"Measured sample rate: {average_sample_rate:.2f} samples/second")

spi.close()