cmake_minimum_required(VERSION 3.15)
project(Load-Balancer)

include_directories(${CMAKE_SOURCE_DIR}/include)


set(SOURCES
    src/main.cpp
    src/load-balancer-interface.cpp
    src/load-balancer-pseudo.cpp
    src/load-balancer-roundrobin.cpp
    src/SocketWrapper.cpp
    src/client-com-handler.cpp
    src/server-com-handler.cpp
)

add_executable(load_balancer ${SOURCES})
target_compile_options(load_balancer PRIVATE -Werror -Wall -Wextra -Wpedantic)