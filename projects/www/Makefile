CXX := emcc

TARGET := boden-www.js
BUILD_DIR := build

SRCS := $(wildcard platform/*.cpp)

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): 
	${CXX} -lembind -o $(BUILD_DIR)/$(TARGET) --js-library platform/library.js ${SRCS}

clean:
	rm -rf $(BUILD_DIR)
