enable_language(CXX)
cmake_minimum_required(VERSION 3.9)

project(TEST LANGUAGES CXX)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB SHARED_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_subdirectory(test)