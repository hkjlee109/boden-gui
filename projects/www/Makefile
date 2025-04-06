CXX := emcc

INCLUDE_DIR := ./platform
SOURCE_DIR := ./platform
BUILD_DIR := ./build

SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)

all: 
	mkdir -p $(BUILD_DIR)
	${CXX} ${SOURCES} -Os -s SIDE_MODULE=1 -s WASM=1 -s -o $(BUILD_DIR)/webgl-renderer.wasm

clean:
	rm -rf $(BUILD_DIR)
