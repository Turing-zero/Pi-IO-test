import pigpio
import time

class SPI_Test:
    def __init__(self, spi_channel=0, baud_rate=500000, spi_flags=0):
        self.pi = pigpio.pi()
        if not self.pi.connected:
            raise Exception("Could not connect to pigpio daemon")
        
        self.spi_channel = spi_channel
        self.baud_rate = baud_rate
        self.spi_flags = spi_flags
        self.spi_handle = self.pi.spi_open(self.spi_channel, self.baud_rate, self.spi_flags)
        
        if self.spi_handle < 0:
            raise Exception("Failed to open SPI channel")
    
    def close(self):
        self.pi.spi_close(self.spi_handle)
        self.pi.stop()
    
    def transfer(self, tx_data):
        count, rx_data = self.pi.spi_xfer(self.spi_handle, tx_data)
        if count < 0:
            raise Exception("SPI transfer failed")
        return rx_data
    
    def measure_transfer_time(self, tx_data, remaining):
        data_to_send = tx_data * remaining
        start_time = time.time()
        self.transfer(data_to_send)
        end_time = time.time()
        transfer_time = end_time - start_time
        return transfer_time

if __name__ == "__main__":
    spi_tester = SPI_Test(spi_channel=0, baud_rate=80000000, spi_flags=0)
    
    try:
        tx_data = [0xF1, 0x20]
        remaining = 10  # 剩余的传输次数
        
        transfer_time = spi_tester.measure_transfer_time(tx_data, remaining)
        
        print(f"Transfer Time: {transfer_time*1000} ms")
    
    finally:
        spi_tester.close()