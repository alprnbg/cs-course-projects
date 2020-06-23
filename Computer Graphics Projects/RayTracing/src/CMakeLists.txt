cmake_minimum_required(VERSION 3.12)
project(RayTracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracing main.cpp Camera.cpp Camera.h utils.cpp utils.h)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(RayTracing ${OpenCV_LIBS})