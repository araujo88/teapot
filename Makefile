CC=g++
STD_FLAGS=-Wall -Wpedantic -Wextra -std=c++20
OPT_FLAGS=-O3 -march=native -flto -funroll-loops
DEBUG_FLAGS=-g -Og -march=native -flto
CC_FLAGS=$(STD_FLAGS) $(OPT_FLAGS)

SRC_DIR=src
HDR_DIR=include
OBJ_DIR=obj

SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

BIN_FILE=teapot

.PHONY: all debug clean

all: CC_FLAGS += $(OPT_FLAGS)
all: $(BIN_FILE)

debug: CC_FLAGS=$(STD_FLAGS) $(DEBUG_FLAGS)
debug: $(BIN_FILE)

$(BIN_FILE): $(OBJ_DIR) $(OBJ_FILES)
	$(CC) $(CC_FLAGS) $(OBJ_FILES) -I$(HDR_DIR) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CC_FLAGS) -c $< -I$(HDR_DIR) -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_FILE) $(OBJ_DIR)
