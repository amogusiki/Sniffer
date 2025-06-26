cmake_minimum_required(VERSION 3.10)
project(Sniffer)

add_executable(sniffer src/main.cpp)

enable_testing()
add_subdirectory(tests)
