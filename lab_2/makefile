CC=gcc
CFLAGS=-std=gnu11 -pedantic -W -Wall -Wextra
LDFLAGS=-I".."

SRC=./parent.c
SRC_CHILD=./child.c
EXE=parent
EXE_CHILD=child
BUILD_DIR=./build

ENV_FILE=./envs


run: build exec clean

exec:
	@echo "\nRunning $(EXE)..."
	CHILD_PATH=./$(BUILD_DIR)/$(EXE_CHILD) ./$(BUILD_DIR)/$(EXE) $(ENV_FILE);

build:
	mkdir build
	$(CC) $(LDFLAGS) $(SRC) -o $(BUILD_DIR)/$(EXE)
	$(CC) $(LDFLAGS) $(SRC_CHILD) -o $(BUILD_DIR)/$(EXE_CHILD)
clean:
	rm -r ./build