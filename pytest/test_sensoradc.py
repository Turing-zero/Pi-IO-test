import ctypes
import os

lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__),"../","build", "libdebugger.so"))

mylib = ctypes.CDLL(lib_path)

mylib.Sensor_ADC_py.restype = ctypes.c_void_p

my_sensor = mylib.Sensor_ADC_py()

mylib.open_sensor_adc_py.argtypes = [ctypes.c_void_p,ctypes.c_int]

mylib.open_sensor_adc_py(my_sensor,0)

mylib.get_adcvalue_py.argtypes = [ctypes.c_void_p,ctypes.c_int,ctypes.c_int]
mylib.get_adcvalue_py.restype = ctypes.c_double

result = mylib.get_adcvalue_py(my_sensor,0,0)

print(f"The result is {result}")
mylib.Sensor_ADC_delete.argtypes = [ctypes.c_void_p]
mylib.Sensor_ADC_delete(my_sensor)
