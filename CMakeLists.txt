cmake_minimum_required(VERSION 3.12)
project(Voxels VERSION 1.0.0)

include_directories(${CMAKE_SOURCE_DIR}/include/OpenGL-Voxel-Test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(voxels src/main.cpp src/chunk.cpp src/block.cpp src/glad.c src/stb_image.cpp)

include_directories("C:/Windows/localbin/include")
link_directories("C:/Windows/localbin/lib")

target_link_libraries(voxels PRIVATE "C:/Windows/localbin/lib/libglfw3.a")
