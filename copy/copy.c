#include "../format/format.h"
#include <stdlib.h>

void copy(FILE* disk_img, char* name) {
    char prefix[] = "./";
    char* filename = NULL;
    FILE* src = fopen(name, "rb");
    MLFS* FS;
    int16_t free_inode = -1;
    FS = malloc(sizeof(MLFS));
    fseek(disk_img, -sizeof(MLFS), SEEK_END);
    fread(FS, sizeof(MLFS), 1, disk_img);

    if(strncmp(name, prefix, 2) != 0) {
        filename = malloc(sizeof(name + 2));
        strcat(filename, prefix);
        strcat(filename, name);
    } else {
        filename = malloc(sizeof(name));
        strcpy(filename, name);
    }
    printf("%s\n", filename);

    for (int i = 0;i < FS->sb.BlockCount;i++) {
        Entry* buffer = NULL;
        buffer = malloc(sizeof(Entry));
        fseek(disk_img, (BLOCK_SIZE * i), SEEK_SET);
        fread(buffer, sizeof(Entry), 1, disk_img);
        if (strcmp(buffer->name, filename) == 0) {
            printf("MLFS: file with this name: %s exists!", filename);
            free(FS);
            return;
        }
        free(buffer);
    }
    

    // find the first free inode
    for(int i = 0;i < FS->sb.iNodeCount;i++) {
        if(FS->inode_table[i].inode_num == -1) {
            free_inode = (10 - i);
        }
    }

    if(free_inode == -1) {
        printf("MLFS: No free inodes!");
        return;
    }

    // read data buffer and size
    char* dataBuffer;
    fseek(src, 0, SEEK_END);
    int dataSize = ftell(src);
    fseek(src, 0, SEEK_SET);
    dataBuffer = malloc(dataSize);
    fread(dataBuffer, sizeof(char), dataSize, src);
    uint16_t needed_blocks = 0;

    if (dataSize % BLOCK_SIZE == 0) {
        needed_blocks = (dataSize / BLOCK_SIZE);
    } else {
        needed_blocks = (dataSize / BLOCK_SIZE + 1);
    }

    // set the free inode attributes
    FS->inode_table[free_inode].inode_num = free_inode;
    FS->inode_table[free_inode].isDirectory = false;
    FS->inode_table[free_inode].size = dataSize;
    FS->inode_table[free_inode].used_blocks = needed_blocks;

    if(FS->sb.free_blocks < needed_blocks) {
        printf("MLFS: not enough data block to write the file into disk!, needed blocks: %hu", needed_blocks);
        fclose(src);
        free(dataBuffer);
        free(FS);
        return;
    }
    FS->sb.free_blocks -= needed_blocks;
    FS->sb.EntryCount++;

    fseek(disk_img, -sizeof(MLFS), SEEK_END);
    fwrite(FS, sizeof(MLFS), 1, disk_img);

    uint16_t freeBlockIndex = 0;
    uint64_t* buffer = NULL;
    for (int i = 0;i < FS->sb.BlockCount;i++) {
        buffer = malloc(8);
        fseek(disk_img, (BLOCK_SIZE * i), SEEK_SET);
        fread(buffer, BLOCK_SIZE, 1, disk_img);
        if(*buffer == 0) {
            freeBlockIndex = i;
            break;
        }
    }
    free(buffer);
    fseek(disk_img, (BLOCK_SIZE * freeBlockIndex), SEEK_SET);


    // write the buffer into the first free data block
    fwrite(dataBuffer, dataSize, 1, disk_img);

    fseek(disk_img, -(sizeof(MLFS) + (sizeof(Entry) * FS->sb.EntryCount)), SEEK_END);

    // Set the directory entry
    Entry file;
    file.inode_num = free_inode;
    strcpy(file.name, filename);
    fwrite(&file, sizeof(Entry), 1, disk_img);

    fclose(src);
    free(dataBuffer);
    free(FS);
}

int main(int argc, char** argv) {
    FILE* img = fopen(argv[1], "rb+");
    if(!img) {
        printf("MLFS: FILE NOT FOUND: build/disk.iso");
        return 1;
    }
    copy(img, argv[2]);
    fclose(img);
}