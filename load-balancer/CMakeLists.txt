cmake_minimum_required(VERSION 3.10)
project(LoadBalancer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

add_executable(load_balancer 
    src/main.cpp
    src/load-balancer-interface.cpp
    src/load-balancer-pseudo.cpp
    src/load-balancer-roundrobin.cpp
    src/client-com-handler.cpp
    src/server-com-handler.cpp
    src/thread-pool.cpp
)

target_include_directories(load_balancer PRIVATE include)

target_include_directories(load_balancer PRIVATE ../SocketWrapperLib/include)

set(SOCKETWRAPPER_LIB "${CMAKE_SOURCE_DIR}/../SocketWrapperLib/build/libSocketWrapper.a")

target_link_libraries(load_balancer PRIVATE ${SOCKETWRAPPER_LIB})

