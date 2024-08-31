import sys
import os
import time
from datetime import datetime
import numpy as np
sys.path.append("../build/")

import debugger

def generate_filename(rootpath):
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = os.path.join(rootpath, "voltages_{}.txt".format(timestamp))
    return filename

if __name__ == "__main__":
    sensor = debugger.Sensor_ADC()
    sensor.open_sensor(0)
    # TODO: may need to config some register
    # for device in range(4):
    #     sensor.config_data_rate(device, debugger.ADS1115_DATA_RATE.DR_860SPS)
    #     # sensor.config_pga(device, debugger.ADS1115_PGA.xxx)

    os.makedirs("../temp", exist_ok=True)
    file_path = generate_filename("../temp")
    
    try:
        with open(file_path, "w") as outFile:
            for i in range(100):
                voltages = []
                for device in range(4):
                    for channel in range(2):
                        voltage = sensor.get_voltage_mux(device, channel)
                        voltages.append(voltage)

                for voltage in voltages:
                    print("{:5.2f}  ".format(voltage), end="")
                    outFile.write("{} ".format(voltage))
                print()
                outFile.write("\n")

                time.sleep(0.001)   # 1 kHz

    except IOError:
        print("cannot open file!")