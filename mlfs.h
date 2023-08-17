#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

#define MLFS_FLAG_READABLE         0x01     // 0b0001
#define MLFS_FLAG_WRITABLE         0x02     // 0b0010
#define MLFS_FLAG_EXECUTABALE      0x04     // 0b0100
#define MLFS_FLAG_HIDDEN           0x08     // 0b1000

typedef struct {
    char path[64];
    int16_t inode_num;
} Entry;

typedef struct {
    uint16_t BlockSize;
    uint16_t BlockCount;
    uint16_t free_blocks;
    int16_t  iNodeCount;
    uint8_t  EntryCount;
    char     FSname[5];
} SuperBlock;

typedef struct {
    bool isDirectory;
    uint32_t size;
    int8_t inode_num;
    uint8_t flags;
    Entry topDir;
} INode;

typedef struct {
    SuperBlock sb;
    INode* inode_table;
} MLFS;
