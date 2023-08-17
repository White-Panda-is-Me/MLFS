CC=gcc
BUILD_DIR=build

.PHONY: format copy disk read

all: disk

disk: $(BUILD_DIR)/disk.iso

$(BUILD_DIR)/disk.iso: format.c copy.c
	$(CC) -std=c99 -Wall format.c -o $(BUILD_DIR)/format
	$(CC) -std=c99 -Wall copy.c -o $(BUILD_DIR)/copy
	$(CC) -std=c99 -Wall dir.c -o $(BUILD_DIR)/dir

format:
	sudo ./$(BUILD_DIR)/format

copy:
	sudo ./$(BUILD_DIR)/copy

read:
	sudo ./$(BUILD_DIR)/read

clean:
	rm $(BUILD_DIR)/disk.iso