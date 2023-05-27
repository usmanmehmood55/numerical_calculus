# Set the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project (NUMERICAL_CALC)

# Set build type to Debug
set(CMAKE_BUILD_TYPE Debug)

# Add library subdirectory
set(NUMERICAL_CALC_INCLUDE_RING_BUFFER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ring_buffer)
add_subdirectory(ring_buffer)
include_directories(${NUMERICAL_CALC_INCLUDE_RING_BUFFER_DIR})

# Add the main file, and name of its executable
add_executable(numerical_calc.exe main.c)

# Link the library to the main file
target_link_libraries(numerical_calc.exe ring_buffer)