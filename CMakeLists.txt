cmake_minimum_required(VERSION 2.8)

project(koku-opengl-framework)
ADD_LIBRARY(koku-opengl-framework lib/window.cpp lib/window.h lib/buffer.cpp lib/buffer.h lib/rendertarget.cpp lib/rendertarget.h lib/compute.cpp lib/compute.h lib/shader.cpp lib/shader.h lib/texture.cpp lib/texture.h)
ADD_EXECUTABLE(koku-opengl-example example/main.cpp)
target_link_libraries(koku-opengl-framework SDL2main SDL2 pthread)
target_link_libraries(koku-opengl-example pthread koku-opengl-framework)

#Sample 1:
ADD_EXECUTABLE(example_1_simple_vertex_buffer example/example_1_simple_vertex_buffer.cpp)
target_link_libraries(example_1_simple_vertex_buffer pthread koku-opengl-framework)

#Sample 2:
ADD_EXECUTABLE(example_2_texture example/example_2_texture.cpp)
target_link_libraries(example_2_texture pthread koku-opengl-framework)

#Sample 3:
ADD_EXECUTABLE(example_3_rendertarget example/example_3_rendertarget.cpp)
target_link_libraries(example_3_rendertarget pthread koku-opengl-framework)

include_directories("${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_CXX_FLAGS "-std=c++0x -ldl -lrt -lGL")
