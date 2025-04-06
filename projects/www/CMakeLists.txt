cmake_minimum_required(VERSION 3.16)

project(boden-www VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES platform/*.cpp platform/*.hpp)

add_executable(boden-www ${SOURCES})
