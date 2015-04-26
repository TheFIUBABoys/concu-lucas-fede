cmake_minimum_required(VERSION 3.1)
project(ConcuTP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
set(FIFOS src/Util/Fifos/Fifo.cpp src/Util/Fifos/Fifo.h src/Util/Fifos/FifoEscritura.cpp src/Util/Fifos/FifoEscritura.h src/Util/Fifos/FifoLectura.cpp src/Util/Fifos/FifoLectura.h)

add_executable(ConcuTP ${SOURCE_FILES} ${FIFOS} src/Domain/Order/Order.cpp src/Domain/Order/Order.h src/Domain/Receptionist/Receptionist.cpp src/Domain/Receptionist/Receptionist.h src/Domain/Cook/Cook.cpp src/Domain/Cook/Cook.h src/Util/Logger/Logger.cpp src/Util/Logger/Logger.h src/Domain/Pizza/Pizza.cpp src/Domain/Pizza/Pizza.h src/Util/Pipes/Pipe.cpp src/Util/Pipes/Pipe.h src/Util/INIReader/ini.h src/Util/INIReader/ini.c src/Util/INIReader/INIReader.h src/Util/INIReader/INIReader.cpp src/Domain/Oven/Oven.h src/Domain/Oven/Oven.cpp src/Domain/Cadet/Cadet.h src/Domain/Cadet/Cadet.cpp src/Domain/Supervisor/Supervisor.h src/Domain/Supervisor/Supervisor.cpp src/Config/Config.h)