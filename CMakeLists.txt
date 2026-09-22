cmake_minimum_required(VERSION 3.10)
project(OS_lab_1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(Creator Creator.cpp)
add_executable(Reporter Reporter.cpp)
add_executable(Main Main.cpp)