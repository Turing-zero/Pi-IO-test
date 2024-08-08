import ctypes
import os

lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__),"../","build", "libmy_library.so"))

mylib = ctypes.CDLL(lib_path)

mylib.Rocker_py.restype = ctypes.c_void_p

my_rocker = mylib.Rocker_py()

mylib.open_rocker_py.argtypes = [ctypes.c_void_p,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_bool]

mylib.open_rocker_py(my_rocker,0,0,0,500000,8,False)

mylib.get_value_py.argtypes = [ctypes.c_void_p,ctypes.c_int]
mylib.get_value_py.restype = ctypes.c_double

result = mylib.get_value_py(my_rocker,0)

print(f"The result is {result}")
mylib.Rocker_delete.argtypes = [ctypes.c_void_p]
mylib.Rocker_delete(my_rocker)
