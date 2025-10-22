CXX := emcc
CXXFLAGS := -Wall -std=c++20

BUILD_DIR := build
TARGET_DIR := public
TARGET := $(TARGET_DIR)/boden-www.js

BODEN_GUI_ROOT := ../../..
BODEN_GUI_SRC_DIR := $(BODEN_GUI_ROOT)/boden-gui
BODEN_GUI_BUILD_DIR := $(BUILD_DIR)/boden-gui
BODEN_GUI_TARGET := $(BODEN_GUI_BUILD_DIR)/libboden-gui.a

APP_DIR := ../../../app
APP_SRCS := $(shell find $(APP_DIR)/ppt -name '*.cpp')
WWW_SRCS := $(shell find platform -name '*.cpp')

all: $(BUILD_DIR) $(BODEN_GUI_TARGET) $(TARGET) copy-resources

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BODEN_GUI_BUILD_DIR)
	mkdir -p $(TARGET_DIR)

$(BODEN_GUI_TARGET): 
	@echo "Creating boden-gui library..."
	emcmake cmake -S $(BODEN_GUI_ROOT) -B $(BODEN_GUI_BUILD_DIR) \
		-DCMAKE_CXX_FLAGS="-Wall -std=c++20 -fdeclspec" 
	make -C $(BODEN_GUI_BUILD_DIR)
	@echo "Created boden-gui successfully."

$(TARGET): 
	@echo "Building boden-www..."
	$(CXX) $(CXXFLAGS) -lembind \
        -sMODULARIZE=1 \
        -sEXPORT_ES6=1 \
        -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap'] \
        -sEXPORTED_FUNCTIONS=['_malloc','_free'] \
        -sNO_DISABLE_EXCEPTION_CATCHING \
        -I$(APP_DIR) \
        -I$(BODEN_GUI_SRC_DIR) \
        -I$(BODEN_GUI_SRC_DIR)/deps/harfbuzz/src \
        -I$(BODEN_GUI_SRC_DIR)/deps/freetype/include \
        -L$(BODEN_GUI_BUILD_DIR)/lib \
        -lboden-gui \
        -lboost_json \
        -lfreetype \
        -lharfbuzz \
        -o $(TARGET) \
        --js-library platform/library.js \
        --preload-file ./images \
        --preload-file ./fonts \
        $(APP_SRCS) \
        $(WWW_SRCS)

copy-resources:
	mkdir -p ./images
	mkdir -p ./fonts
	cp -r $(APP_DIR)/ppt/resources/images/* ./images/
	cp -r $(APP_DIR)/ppt/resources/fonts/* ./fonts/

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(TARGET_DIR)
	rm -rf images
	rm -rf fonts

.PHONY: all clean $(TARGET)
