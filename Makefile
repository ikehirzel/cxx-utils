CXX := g++
CXXFLAGS := -shared -fPIC -O2
BIN_DIR := bin
LIB_DIR := $(BIN_DIR)/lib
INC_DIR := $(BIN_DIR)/include
OUTPUT	:= $(LIB_DIR)/libhlog.so
SOURCES := hlog.cpp
HEADERS := hlog.h

$(OUTPUT):
	@echo "Building $@..."
	@$(shell mkdir -p $(INC_DIR) $(LIB_DIR))
	@$(CXX) $(CXXFLAGS) $(SOURCES) -o $@
	@$(shell cp $(HEADERS) $(INC_DIR)/$(HEADERS))

clean:
	@rm -r $(BIN_DIR)
