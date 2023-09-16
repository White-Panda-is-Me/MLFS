#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef uint8_t bool;

#define true 1
#define false 0

#define PACKED                     __attribute__((packed))

#define BLOCK_SIZE                 512
#define INODE_COUNT                512
#define MLFS_FLAG_READABLE         0x01
#define MLFS_FLAG_WRITABLE         0x02
#define MLFS_FLAG_EXECUTABALE      0x04
#define MLFS_FLAG_HIDDEN           0x08
#define MLFS_FLAG_FOLDER           0x10

#define MLFS_FLAG_UNUSED           0x00

typedef struct {
    uint8_t  JumpInstruction[3];
    uint16_t mlfs_bps;
    uint16_t mlfs_total_blocks;
    char  mlfs_magic_word[4];
    uint8_t  mlfs_max_root_dir_entries;
    uint8_t  mlfs_reserved_blocks;
    uint8_t  mlfs_spt;
    uint8_t  mlfs_heads;
    uint16_t mlfs_cylinders;
} PACKED MLFS_Header;

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
    uint32_t StartBlockEntries[128]; // 128 max entries, each have 4 bytes wide for inode_num
} PACKED Root_Entries;

typedef struct {
    uint32_t inode_num;
    uint8_t flags;
    uint16_t CreatedDate;
    uint16_t CreatedTime;
} PACKED INode;

typedef struct {
    char name[120];
    uint32_t blockStart;
    uint32_t fileSize;
} PACKED File;

typedef struct {
    uint8_t name[123];
    uint32_t entries[95];
} PACKED Folder;

typedef struct {
    INode inode;
    union {
        File file;
        Folder folder;
    } Info;
} PACKED Entry;

bool isBlockFree(uint64_t BlockNum, FILE* file);
uint32_t findFreeBlocks(uint32_t NeededBlocks, FILE* file, MLFS_Header* header);
char* Path2Name(char* dir);