cmake_minimum_required(VERSION 2.8)
project(project2)

set(CMAKE_CXX_STANDARD 14)

add_executable(project2 main.cpp passenger.h passenger.cpp simulation.cpp simulation.h utilities.h utilities.cpp)