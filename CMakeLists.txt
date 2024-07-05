enable_language(CXX)

file(GLOB TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp
)

foreach(test_file ${TEST_FILES})
    file(RELATIVE_PATH test_file_name ${CMAKE_CURRENT_SOURCE_DIR} ${test_file})
    string( REPLACE ".cpp" "" test_name ${test_file_name} )

    add_executable(${test_name} ${test_file_name})
    target_link_libraries(${test_name} mraa stdc++)
endforeach()