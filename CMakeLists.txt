cmake_minimum_required(VERSION 3.0.0)
project(labwork1 VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "CMAKE_CXX_FLAGS "-Wall -Wextra -Wuninitialized -Wshadow -Wno-unused-result -std=c++17")
add_executable(labwork1 main.cpp)
