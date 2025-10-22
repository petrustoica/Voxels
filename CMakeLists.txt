cmake_minimum_required(VERSION 3.12)
project(Voxels VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g")

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.2
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(${CMAKE_SOURCE_DIR}/include/OpenGL-Voxel-Test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(voxels src/main.cpp src/world.cpp src/chunk.cpp src/block.cpp src/glad.c src/stb_image.cpp)

target_link_libraries(voxels PRIVATE glfw glm nlohmann_json::nlohmann_json)
