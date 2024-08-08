enable_language(CXX)
cmake_minimum_required(VERSION 3.9)

project(TEST LANGUAGES CXX)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB SHARED_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(my_library SHARED ${SHARED_SOURCE_FILES})

target_link_libraries(my_library mraa stdc++)

install(TARGETS my_library DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)

add_subdirectory(test)