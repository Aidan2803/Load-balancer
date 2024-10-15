cmake_minimum_required(VERSION 3.10)
project(SocketWrapperLib)

find_package(spdlog REQUIRED)

add_library(SocketWrapper STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/SocketWrapper.cpp)
target_include_directories(SocketWrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(SocketWrapper PUBLIC spdlog::spdlog)
