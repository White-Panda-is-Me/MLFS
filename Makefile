CC=gcc
BUILD_DIR=build
DISK_DIR=disk

tools: format copy

format: format.c
	mkdir -p $(BUILD_DIR)
	dd if=/dev/zero of=disk/disk.img bs=1 count=1
	$(CC) -Wall -Wextra -Wno-unused-parameter -o $(BUILD_DIR)/format format.c

copy: copy.c mlfs.c
	$(CC) -Wall -Wextra -Wno-unused-parameter -o $(BUILD_DIR)/copy copy.c mlfs.c

clean:
	rm $(BUILD_DIR)/*
	rm $(DISK_DIR)/*