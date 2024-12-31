# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language (C or C++)
project(C_StateMachine VERSION 1.0 LANGUAGES C CXX)

add_subdirectory(FiniteStateMachine)
add_subdirectory(Motor)
add_subdirectory(main)
