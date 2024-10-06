cmake_minimum_required(VERSION 3.8)

Project(boden-gui VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES *.cpp *.hpp)

add_library(boden-gui ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
    .
    deps/metal-cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    "-framework Metal"
    "-framework MetalKit"
)
