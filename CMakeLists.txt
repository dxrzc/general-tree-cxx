cmake_minimum_required(VERSION 3.10)

project(GeneralTreeTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
add_executable(GeneralTreeTests ${TEST_SOURCES})
target_include_directories(
        GeneralTreeTests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}        
)


enable_testing()
add_test(NAME GeneralTreeTests COMMAND GeneralTreeTests)