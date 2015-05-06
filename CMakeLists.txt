cmake_minimum_required(VERSION 3.1)
project(ConcuTP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
set(FIFOS src/Util/Fifos/Fifo.cpp src/Util/Fifos/Fifo.h src/Util/Fifos/FifoEscritura.cpp src/Util/Fifos/FifoEscritura.h src/Util/Fifos/FifoLectura.cpp src/Util/Fifos/FifoLectura.h)
set(SHARED_MEMORY src/Util/MemoriaCompartida/MemoriaCompartida.h src/Util/MemoriaCompartida/MemoriaCompartida2.h)
set(LOCK_FILE src/Util/Locks/LockFile.h src/Util/Locks/LockFile.cpp)
set(SEMAPHORE src/Util/Semaforo/Semaforo.h src/Util/Semaforo/Semaforo.cpp)
set(SIGNALS src/Util/Seniales/EventHandler.h src/Util/Seniales/SIGINT_Handler.h src/Util/Seniales/SignalHandler.cpp src/Util/Seniales/SignalHandler.h)

add_executable(ConcuTP ${SOURCE_FILES} ${FIFOS} ${SHARED_MEMORY} ${LOCK_FILE} ${SIGNALS} ${SEMAPHORE} src/Domain/Order/Order.cpp src/Domain/Order/Order.h src/Domain/Receptionist/Receptionist.cpp src/Domain/Receptionist/Receptionist.h src/Domain/Cook/Cook.cpp src/Domain/Cook/Cook.h src/Util/Logger/Logger.cpp src/Util/Logger/Logger.h src/Domain/Pizza/Pizza.cpp src/Domain/Pizza/Pizza.h src/Util/Pipes/Pipe.cpp src/Util/Pipes/Pipe.h src/Util/INIReader/ini.h src/Util/INIReader/ini.c src/Util/INIReader/INIReader.h src/Util/INIReader/INIReader.cpp src/Domain/Oven/Oven.h src/Domain/Oven/Oven.cpp src/Domain/Cadet/Cadet.h src/Domain/Cadet/Cadet.cpp src/Domain/Supervisor/Supervisor.h src/Domain/Supervisor/Supervisor.cpp src/Config/Config.h src/Util/Locks/LockFile.h src/Util/Locks/LockFile.cpp)