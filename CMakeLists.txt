cmake_minimum_required(VERSION 3.0)

project(collatz)

add_executable(collatz main.cpp)

install(TARGETS collatz RUNTIME DESTINATION bin)
