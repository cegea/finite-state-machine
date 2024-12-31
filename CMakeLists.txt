# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language (C or C++)
project(C_StateMachine VERSION 1.0 LANGUAGES C CXX)

include_directories(include)

# Collect all source files in the current directory
file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/include/*.h"
)

# Add an executable target
add_executable(C_StateMachineApp ${SOURCES})
