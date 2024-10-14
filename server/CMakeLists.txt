cmake_minimum_required(VERSION 3.10)
project(HTTPServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

find_package(spdlog REQUIRED)

add_executable(HTTPServer http-server.cpp)

target_include_directories(HTTPServer PRIVATE ../SocketWrapperLib/include)

set(SOCKETWRAPPER_LIB "${CMAKE_SOURCE_DIR}/../SocketWrapperLib/build/libSocketWrapper.a")
target_link_libraries(HTTPServer PRIVATE ${SOCKETWRAPPER_LIB} spdlog::spdlog)
