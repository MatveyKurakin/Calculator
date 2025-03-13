cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 20)

project(Calculator)

include_directories(include)

add_executable(Calculator src/main.cpp include/calculator.h include/lexema.h include/polynom.h)
