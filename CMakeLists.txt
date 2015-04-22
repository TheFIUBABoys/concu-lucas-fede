cmake_minimum_required(VERSION 3.1)
project(ConcuTP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(ConcuTP ${SOURCE_FILES} Order/Order.cpp Order/Order.h Receptionist/Receptionist.cpp Receptionist/Receptionist.h)