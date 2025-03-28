cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE SOURCES *.cpp)

add_executable(tests ${SOURCES})

target_link_libraries(tests PUBLIC
    gtest_main
    boden-gui
)

add_test(
    NAME tests
    COMMAND tests --gtest_color=yes
)
