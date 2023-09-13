#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PACKED                     __attribute__((packed))

#define BLOCK_SIZE                 512
#define MLFS_FLAG_READABLE         0x01
#define MLFS_FLAG_WRITABLE         0x02
#define MLFS_FLAG_EXECUTABALE      0x04
#define MLFS_FLAG_HIDDEN           0x08
#define MLFS_FLAG_FOLDER           0x10

#define MLFS_FLAG_UNUSED           0x00

typedef struct {
    uint16_t mlfs_bps;
    uint16_t mlfs_total_blocks;
    uint8_t  mlfs_magic_word[4];
    uint8_t  mlfs_max_root_dir_entries;
    uint8_t  mlfs_reserved_blocks;
    uint8_t  mlfs_spt;
    uint8_t  mlfs_heads;
    uint16_t mlfs_cylinders;
} PACKED SuperBlock;

typedef struct {
    uint8_t entries[4][32]; // 32 max entries, each have 4 bytes wide for inode_num
} PACKED Root_Entries;

typedef struct {
    uint8_t name[120];
    uint32_t blockStart;
    uint32_t fileSize;
} PACKED File;

typedef struct {
    uint32_t inode_num;
    uint8_t name[124];
    uint8_t entries[4][32];
} PACKED Folder;

typedef struct {
    uint32_t inode_num;
    uint8_t flags;
    uint16_t date;
    uint16_t time;
} PACKED INode;

typedef struct {
    uint32_t inode_num;
    union {
        File file;
        Folder folder;
    } entry;
} PACKED Entry;

typedef struct {
    SuperBlock sb;
} MLFS;

bool isBlockFree(uint64_t BlockNum, FILE* file);
uint32_t findFreeBlocks(uint32_t NeededBlocks, FILE* file, MLFS FS);
char* Path2Name(char* dir);
// bool dir_exist(FILE* disk, MLFS* FS, char* dir);

