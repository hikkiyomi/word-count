cmake_minimum_required(VERSION 3.0.0)
project(WordCount VERSION 0.1.0)

add_executable(WordCount main.cpp)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wuninitialized -Wshadow -Wno-unused-result -std=c++17")