import ctypes
import os
import configparser
# import test_udpwifi

lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__),"../","build", "libdebugger.so"))

mylib = ctypes.CDLL(lib_path)

class Pair(ctypes.Structure):
    _fields_ = [("first",ctypes.c_double),
                ("second",ctypes.c_double)
    ]

config = configparser.ConfigParser()
config.read('../config.ini')

freq = int(config['rocker']['spi_freq'])
spi_bus = int(config['rocker']['spi_bus'])
spi_cs = int(config['rocker']['spi_cs'])
spi_mode = int(config['rocker']['spi_mode'])
bit_per_word = int(config['rocker']['bit_per_word'])
lsb_mode = config['rocker']['lsb_mode'] == "true"

mylib.Rocker_py.restype = ctypes.c_void_p

my_rocker = mylib.Rocker_py()

mylib.open_rocker_py.argtypes = [ctypes.c_void_p,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_bool]

mylib.open_rocker_py(my_rocker,spi_bus,spi_cs,spi_mode,freq,8,False)

# mylib.get_value_py.argtypes = [ctypes.c_void_p,ctypes.c_int]
# mylib.get_value_py.restype = ctypes.c_double

# mylib.get_adcvalue.argtypes = [ctypes.c_void_p,ctypes.c_int,ctypes.c_int]
# mylib.get_adcvalue.restype = Pair#[ctypes.c_double,ctypes.c_double]

# result = mylib.get_adcvalue(my_rocker,0,0)

mylib.get_position_py.argtypes = [ctypes.c_void_p,ctypes.c_double]
mylib.get_position_py.restype = Pair

result = mylib.get_position_py(my_rocker,5)

# print(result.first)
# print(result.second)

print(freq)
print(spi_bus)
print(spi_cs)
print(bit_per_word)
print(lsb_mode)

mylib.Rocker_delete.argtypes = [ctypes.c_void_p]
mylib.Rocker_delete(my_rocker)

# while True:
#     data,addr = test_udpwifi.recv_data()
#     print(data.decode('utf-8'))