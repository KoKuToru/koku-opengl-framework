cmake_minimum_required(VERSION 2.8)

project(koku-opengl-framework)
ADD_EXECUTABLE(koku-opengl-framework main.cpp window.cpp window.h)
target_link_libraries(koku-opengl-framework SDL2main SDL2 pthread)

set(CMAKE_CXX_FLAGS "-std=c++0x -ldl -lrt")
