cmake_minimum_required(VERSION 3.12)
project(Voxels VERSION 1.0.0)

include_directories(${CMAKE_SOURCE_DIR}/include/OpenGL-Voxel-Test)

add_executable(voxels src/main.cpp src/glad.c src/stb_image.cpp)

target_link_libraries(voxels PRIVATE glfw)
