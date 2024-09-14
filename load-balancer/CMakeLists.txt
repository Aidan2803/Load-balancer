cmake_minimum_required(VERSION 3.15)
project(Load-Balancer)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/load-balancer.cpp
    src/SocketWrapper.cpp
)

add_executable(load_balancer ${SOURCES})