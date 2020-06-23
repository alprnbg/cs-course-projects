cmake_minimum_required(VERSION 3.12)
project(RecursiveRayTracing)

set(CMAKE_CXX_STANDARD 14)

add_executable(RecursiveRayTracing main.cpp Camera.cpp Camera.h utils.cpp utils.h Render.cpp Render.h)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(RecursiveRayTracing ${OpenCV_LIBS})