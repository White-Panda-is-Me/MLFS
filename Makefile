CC=gcc
BUILD_DIR=build

.PHONY: format copy disk read

all: disk

disk: $(BUILD_DIR)/disk.iso

$(BUILD_DIR)/disk.iso: format/format.c copy/copy.c
	$(CC) -std=c99 -Wall format/format.c -o build/format
	$(CC) -std=c99 -Wall copy/copy.c -o build/copy
	$(CC) -std=c99 -Wall read/read.c -o build/read

format:
	sudo ./$(BUILD_DIR)/format

copy:
	sudo ./$(BUILD_DIR)/copy

read:
	sudo ./$(BUILD_DIR)/read

clean:
	rm $(BUILD_DIR)/disk.iso