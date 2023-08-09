#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define BLOCK_SIZE 512

typedef struct {
    char name[32];
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
    int16_t inode_num;
    uint32_t used_blocks;
} INode;

typedef struct {
    SuperBlock sb;
    INode inode_table[10];
} MLFS;