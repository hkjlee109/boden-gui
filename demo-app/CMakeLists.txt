cmake_minimum_required(VERSION 3.8)

Project(demo-app VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)

# find_library(COCOA_LIBRARY Cocoa)
# find_library(COCOA_LIBRARY Cocoa)

set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
set(MACOSX_BUNDLE_COPYRIGHT "© 2024 by Kyung")
set(MACOSX_BUNDLE_INFO_STRING "The demo app for BodenGUI")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.kartoffelsoft.boden-gui")

file(GLOB_RECURSE SOURCES *.mm *.h)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})
# target_link_libraries(${PROJECT_NAME} ${COCOA_LIBRARY})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ../boden-gui/deps/metal-cpp
)

target_link_libraries(${PROJECT_NAME}
        "boden-gui"
        "-framework Metal"
        "-framework MetalKit"
        "-framework AppKit"
        "-framework Foundation"
        "-framework QuartzCore"
        )
