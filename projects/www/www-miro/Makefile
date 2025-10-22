CXX := emcc
CXXFLAGS := -Wall -std=c++20

BUILD_DIR := build

BODEN_GUI_ROOT := ../../..
BODEN_GUI_SRC_DIR := $(BODEN_GUI_ROOT)/boden-gui
BODEN_GUI_BUILD_DIR := $(BUILD_DIR)/boden-gui
BODEN_GUI_LIB := $(BODEN_GUI_BUILD_DIR)/libboden-gui.a

GUI_BACKEND_BUILD_DIR := $(BUILD_DIR)/wasm
GUI_BACKEND := $(GUI_BACKEND_BUILD_DIR)/gui-backend.js

APP_DIR := ../../../app
APP_SRCS := $(shell find $(APP_DIR)/miro -name '*.cpp')
WWW_SRCS := $(shell find src/gui-backend -name '*.cpp')

all: copy-resources $(BUILD_DIR) $(BODEN_GUI_LIB) $(GUI_BACKEND)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BODEN_GUI_BUILD_DIR)
	mkdir -p $(GUI_BACKEND_BUILD_DIR)
	mkdir -p ./public/wasm/

$(BODEN_GUI_LIB): 
	@echo "Creating boden-gui library..."
	emcmake cmake -S $(BODEN_GUI_ROOT) -B $(BODEN_GUI_BUILD_DIR) \
		-DCMAKE_CXX_FLAGS="-Wall -std=c++20 -fdeclspec" 
	make -C $(BODEN_GUI_BUILD_DIR)
	@echo "Created boden-gui successfully."

$(GUI_BACKEND): 
	@echo "Building gui-backend..."
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
        -o $(GUI_BACKEND) \
        --js-library src/gui-backend/library.js \
        --preload-file ./src/gui-backend/assets \
        $(APP_SRCS) \
        $(WWW_SRCS)
	cp $(GUI_BACKEND_BUILD_DIR)/gui-backend.js ./src/frontend/workers/
	cp $(GUI_BACKEND_BUILD_DIR)/gui-backend.wasm ./public/wasm/
	cp $(GUI_BACKEND_BUILD_DIR)/gui-backend.data ./public/wasm/

copy-resources:
	mkdir -p ./src/gui-backend/assets
	cp -r $(APP_DIR)/miro/resources/images ./src/gui-backend/assets
	cp -r $(APP_DIR)/miro/resources/fonts ./src/gui-backend/assets

clean:
	rm -rf $(BUILD_DIR)
	rm -rf ./public/wasm
	rm -rf ./src/frontend/workers/gui-backend.js
	rm -rf ./src/gui-backend/assets 

.PHONY: all clean $(GUI_BACKEND)
