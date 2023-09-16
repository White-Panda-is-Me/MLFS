#include "mlfs.h"

bool isBlockFree(uint64_t BlockNum, FILE* file) {
    fseek(file, BLOCK_SIZE * BlockNum, SEEK_SET);
    uint64_t buffer;
    for(uint8_t i = 0;i < 64;i++) {
        fread(&buffer, 8, 1, file);
        if(buffer != (uint64_t) 0) {
            return false;
        }
    }
    return true;
}

uint32_t findFreeBlocks(uint32_t NeededBlocks, FILE* file, MLFS_Header* header) {
    fseek(file, 0, SEEK_SET);
    bool found = false;
    for(uint32_t i = header->mlfs_reserved_blocks;i < header->mlfs_total_blocks;i++) {
        if(found) {
            for(uint32_t j = 0;j < NeededBlocks;j++) {
                if(!isBlockFree(i + j, file))
                    found = false;
                    break;
            }
            return i - 1;
            break;
        }
        if(isBlockFree(i, file)) {
            found = true;
            printf("found free block at: %u\n", i);
        }
        fseek(file, BLOCK_SIZE, SEEK_CUR);
    }
    #define NO_BLOCK_FOUND
    return 0;
}

char* Path2Name(char* dir) {
    int charIndex = -1;
    int i = 0;
    while (*dir) {
        if(*dir == '/')
            charIndex = i + 1;
        dir++;
        i++;
    }
    dir -= i;
    if(charIndex == -1)
        return dir;
    dir += charIndex;
    return dir;
}

// bool dir_exist(FILE* disk, MLFS* FS, char* dir) {
//     for(int i = 0;i < 32 && *dir;i++) {
//         for (int j = 0;j < FS->sb.EntryCount;j++) {
//             fseek(disk, -sizeof(Entry), SEEK_END);
//             fread(entryBuffer, sizeof(Entry), 1, disk);
//             if(entryBuffer->path == dir)
//                 free(entryBuffer);
//                 return true;
//         }
//         dir++;
//     }
//     free(entryBuffer);
//     return false;
// }