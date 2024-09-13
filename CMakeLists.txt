cmake_minimum_required(VERSION 3.13)

project(grep-starter-cpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

# Add the current directory as the include path
include_directories($(CMAKE_SOURCE_DIR))

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

add_executable(exe ${SOURCE_FILES})