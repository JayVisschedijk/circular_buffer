# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
project(circular_buffer)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set (CMAKE_C_FLAGS "-g -std=c17 ${CMAKE_C_FLAGS}")

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
