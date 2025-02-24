cmake_minimum_required(VERSION 3.8)

project(boden-gui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(boden-gui)
add_subdirectory(demo-app)
