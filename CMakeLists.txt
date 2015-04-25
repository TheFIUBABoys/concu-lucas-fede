cmake_minimum_required(VERSION 3.1)
project(ConcuTP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(INI_FILE "config_file.cfg")


set(SOURCE_FILES main.cpp)
add_executable(ConcuTP ${SOURCE_FILES} src/Domain/Order/Order.cpp src/Domain/Order/Order.h src/Domain/Receptionist/Receptionist.cpp src/Domain/Receptionist/Receptionist.h src/Domain/Cook/Cook.cpp src/Domain/Cook/Cook.h src/Util/Logger/Logger.cpp src/Util/Logger/Logger.h src/Domain/Pizza/Pizza.cpp src/Domain/Pizza/Pizza.h src/Util/Pipes/Pipe.cpp src/Util/Pipes/Pipe.h src/Util/INIReader/ini.h src/Util/INIReader/ini.c src/Util/INIReader/INIReader.h src/Util/INIReader/INIReader.cpp)

