import ctypes
import os
import configparser

lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__),"../","build", "libdebugger.so"))

mylib = ctypes.CDLL(lib_path)

config = configparser.ConfigParser()
config.read('../config.ini')

freq = int(config['rocker']['spi_freq'])
spi_bus = int(config['rocker']['spi_bus'])
spi_cs = int(config['rocker']['spi_cs'])
spi_mode = int(config['rocker']['spi_mode'])
bit_per_word = int(config['rocker']['bit_per_word'])
lsb_mode = config['rocker']['lsb_mode'] == "true"

mylib.Button_py.restype = ctypes.c_void_p

my_sensor = mylib.Button_py()

mylib.open_button_py.argtypes = [ctypes.c_void_p,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_bool]

mylib.open_button_py(my_sensor,spi_bus,spi_cs,spi_mode,freq,bit_per_word,lsb_mode)

mylib.get_status_py.argtypes = [ctypes.c_void_p,ctypes.c_int]
mylib.get_status_py.restype = ctypes.c_double

result = mylib.get_status_py(my_sensor,2)

print(f"The result is {result}")
mylib.Button_delete.argtypes = [ctypes.c_void_p]
mylib.Button_delete(my_sensor)
