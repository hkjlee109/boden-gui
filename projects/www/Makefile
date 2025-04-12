CXX := g++
CXXFLAGS := -Wall -std=c++20
EMCC := emcc

BODEN_GUI_LIB = ../../build/lib/libboden-gui.a
BODEN_GUI_LIB_INCLUDE_DIR = ../../boden-gui

APP_DIR := ../../app/app
APP_BUILD_DIR := build/app
BUILD_DIR := build

APP_TARGET := $(APP_BUILD_DIR)/libapp
WWW_TARGET := $(BUILD_DIR)/boden-www.js

APP_SRCS := $(wildcard $(APP_DIR)/*.cpp)
APP_OBJS := $(APP_SRCS:$(APP_DIR)/%.cpp=$(APP_BUILD_DIR)/%.o)

WWW_SRCS := $(wildcard platform/*.cpp)

all: $(BUILD_DIR) $(APP_TARGET) $(WWW_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(APP_BUILD_DIR)

$(APP_TARGET): $(APP_OBJS) $(BODEN_GUI_LIB)
	@echo "Creating app library..."
	ar rcs $@ $^ 
	@echo "Created $@ successfully."

$(APP_BUILD_DIR)/%.o: $(APP_DIR)/%.cpp
	@echo "Compiling $< ..."
	$(CXX) $(CXXFLAGS) -I$(BODEN_GUI_LIB_INCLUDE_DIR) -c $< -o $@
	@echo "Compiled $< successfully."

$(WWW_TARGET): $(BODEN_GUI_LIB)
	@echo "Building boden-www..."
	${EMCC} -lembind -I$(BODEN_GUI_LIB_INCLUDE_DIR) -L../../build/lib/ -lboden-gui -o $(WWW_TARGET) --js-library platform/library.js ${WWW_SRCS}

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
