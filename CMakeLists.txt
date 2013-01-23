cmake_minimum_required(VERSION 2.8)

project(koku-opengl-framework)
ADD_LIBRARY(koku-opengl-framework lib/window.cpp lib/window.h)
ADD_EXECUTABLE(koku-opengl-example example/main.cpp)
target_link_libraries(koku-opengl-framework SDL2main SDL2 pthread)
target_link_libraries(koku-opengl-example pthread koku-opengl-framework)
include_directories("${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_CXX_FLAGS "-std=c++0x -ldl -lrt -lGL")
