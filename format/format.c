#include "format.h"

void format(FILE* disk_img, uint16_t block_count, uint8_t inode_count) {
    if(!disk_img) {
        printf("MLFS: Could not open file!\n");
        return;
    }

    // Defining fs and its superblock
    MLFS fs;
    fs.sb.BlockCount  = block_count;
    fs.sb.BlockSize   = BLOCK_SIZE;
    fs.sb.free_blocks = block_count - 1;
    fs.sb.iNodeCount  = inode_count;
    fs.sb.EntryCount  = 0;
    strcpy(fs.sb.FSname, "MLFS");

    // setting all inodes as free
    for(int i = 0;i < inode_count;i++) {
        fs.inode_table[i].inode_num = -1;
    }

    // setting all blocks as 0 (free)
    char data_block[BLOCK_SIZE] = {0};
    for (int i = 0; i < block_count; i++) {
        fwrite(data_block, BLOCK_SIZE, 1, disk_img);
    }

    // root directory inode
    fs.inode_table[0].isDirectory = true;
    fs.inode_table[0].size        = sizeof(Entry);
    fs.inode_table[0].inode_num   = 0;
    fs.inode_table[0].used_blocks = 1;
    fseek(disk_img, 0, SEEK_END);
    fwrite(&fs, sizeof(MLFS), 1, disk_img);

    printf("MLFS: Initialized to disk image!\n");
}

int main(int argc, char** argv) {
    FILE* disk_img = fopen(argv[1], "wb");
    format(disk_img, 8196, 10);
    fclose(disk_img);
}
