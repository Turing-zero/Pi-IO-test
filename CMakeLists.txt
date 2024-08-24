enable_language(CXX)
cmake_minimum_required(VERSION 3.9)

project(TEST LANGUAGES CXX)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# source files
file(GLOB SHARED_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
list(FILTER SHARED_SOURCE_FILES EXCLUDE REGEX "py_module.*.cpp$")

# py_module files
file(GLOB PY_MODULE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/py_module*.cpp
)

add_library(mraa SHARED IMPORTED)
set_property(TARGET mraa PROPERTY IMPORTED_LOCATION /usr/local/lib/libmraa.so)

add_subdirectory(extern/pybind11)   # OR find_package(pybind11 REQUIRED)
pybind11_add_module(debugger ${SHARED_SOURCE_FILES} ${PY_MODULE_FILES})
target_link_libraries(debugger PRIVATE pybind11::module mraa)

add_subdirectory(test)