cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW) # Vendor-neutral OpenGL
project(Lab5OpenGL)

set(CMAKE_CXX_STANDARD 17)

# Libs
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Source
add_executable(
    zad1_OpenGL
    src/main.cpp
    src/shader.cpp
    src/geometry.cpp
)

target_include_directories(zad1_OpenGL PRIVATE include)
target_compile_options(zad1_OpenGL PRIVATE -Wall -Wextra -Wpedantic)

# Link libs
target_link_libraries(zad1_OpenGL
    OpenGL::GL
    glfw
    GLEW::GLEW
)