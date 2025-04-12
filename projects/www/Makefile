CXX := emcc
CXXFLAGS := -Wall -std=c++20

BUILD_DIR := build
TARGET := $(BUILD_DIR)/boden-www.js

BODEN_GUI_DIR := ../../boden-gui
BODEN_GUI_BUILD_DIR := $(BUILD_DIR)/boden-gui
BODEN_GUI_TARGET := $(BODEN_GUI_BUILD_DIR)/libboden-gui.a

APP_DIR := ../../app
APP_SRCS := $(wildcard $(APP_DIR)/**/*.cpp)

WWW_SRCS := $(wildcard platform/*.cpp)

all: $(BUILD_DIR) $(BODEN_GUI_TARGET) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BODEN_GUI_BUILD_DIR)

$(BODEN_GUI_TARGET): 
	@echo "Creating boden-gui library..."
	emcmake cmake -S $(BODEN_GUI_DIR) -B $(BODEN_GUI_BUILD_DIR)
	make -C $(BODEN_GUI_BUILD_DIR)
	@echo "Created boden-gui successfully."

$(TARGET): 
	@echo "Building boden-www..."
	$(CXX) -lembind \
        -I$(BODEN_GUI_DIR) \
        -I$(APP_DIR) \
        -L$(BODEN_GUI_BUILD_DIR) \
        -lboden-gui \
        -o $(TARGET) \
        --js-library platform/library.js \
        $(APP_SRCS) \
        $(WWW_SRCS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean $(TARGET)
