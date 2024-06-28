enable_language(CXX)


add_executable (uart uart.cpp)
# add_executable(gpio gpio.cpp)

target_link_libraries (uart mraa stdc++)
# target_link_libraries (gpio ${LibMRAA} stdc++)