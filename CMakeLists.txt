cmake_minimum_required(VERSION 3.24)
project(time_measure)

set(CMAKE_CXX_STANDARD 17)

add_executable(time_measure main.cpp tests.cpp user.cpp)
