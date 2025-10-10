cmake_minimum_required(VERSION 3.16)

Project(boden-gui VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)

include("cmake/boost-external-project.cmake")
include("cmake/freetype-external-project.cmake")
include("cmake/harfbuzz-external-project.cmake")

file(GLOB_RECURSE SOURCES boden/*.cpp boden/*.hpp)

add_library(boden-gui ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE boost_json freetype harfbuzz 
)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        .
        ${PROJECT_SOURCE_DIR}/deps/harfbuzz/src
        ${PROJECT_SOURCE_DIR}/deps/freetype/include
        ${PROJECT_BINARY_DIR}/deps/freetype/include
)

add_subdirectory(tools/googletest)
add_subdirectory(tests)
