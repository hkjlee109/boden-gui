CXX := emcc
CXXFLAGS := -Wall -std=c++20

BUILD_DIR := build

BODEN_GUI_DIR := ../../boden-gui
BODEN_GUI_BUILD_DIR := $(BUILD_DIR)/boden-gui
BODEN_GUI_TARGET := $(BODEN_GUI_BUILD_DIR)/libboden-gui.a

APP_DIR := ../../app/app
APP_BUILD_DIR := $(BUILD_DIR)/app
APP_TARGET := $(APP_BUILD_DIR)/libapp
APP_SRCS := $(wildcard $(APP_DIR)/*.cpp)
APP_OBJS := $(APP_SRCS:$(APP_DIR)/%.cpp=$(APP_BUILD_DIR)/%.o)

WWW_TARGET := $(BUILD_DIR)/boden-www.js
WWW_SRCS := $(wildcard platform/*.cpp)

all: $(BUILD_DIR) $(BODEN_GUI_TARGET) $(APP_TARGET) $(WWW_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BODEN_GUI_BUILD_DIR)
	mkdir -p $(APP_BUILD_DIR)

$(BODEN_GUI_TARGET): 
	@echo "Creating boden-gui library..."
	emcmake cmake -S $(BODEN_GUI_DIR) -B $(BODEN_GUI_BUILD_DIR)
	make -C $(BODEN_GUI_BUILD_DIR)
	@echo "Created boden-gui successfully."

$(APP_TARGET): $(APP_OBJS)
	@echo "Creating app library..."
	ar rcs $@ $^ 
	@echo "Created $@ successfully."

$(APP_BUILD_DIR)/%.o: $(APP_DIR)/%.cpp
	@echo "Compiling $< ..."
	$(CXX) $(CXXFLAGS) -I$(BODEN_GUI_DIR) -c $< -o $@
	@echo "Compiled $< successfully."

$(WWW_TARGET): 
	@echo "Building boden-www..."
	$(CXX) -lembind -I$(BODEN_GUI_DIR) -I../../app -L$(BODEN_GUI_BUILD_DIR) -lboden-gui -o $(WWW_TARGET) --js-library platform/library.js ${WWW_SRCS}

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean $(APP_TARGET) $(WWW_TARGET)
