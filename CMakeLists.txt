cmake_minimum_required(VERSION 3.13)

project(grep-starter-cpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

# Add include directories
include_directories(${CMAKE_SOURCE_DIR})

# Recursively gather source files
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

# Create the executable
add_executable(exe ${SOURCE_FILES})