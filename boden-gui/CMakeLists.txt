cmake_minimum_required(VERSION 3.16)

Project(boden-gui VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES boden/*.cpp boden/*.hpp)

add_library(boden-gui ${SOURCES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC .
)

add_subdirectory(tools/googletest)
add_subdirectory(tests)
