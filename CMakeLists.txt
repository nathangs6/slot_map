cmake_minimum_required(VERSION 3.28.1)

project(SlotMap VERSION 1.0 DESCRIPTION "Slot map in C++" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)


enable_testing()
add_subdirectory(src)
add_subdirectory(tests)
