#include "mlfs.h"

void format(FILE* disk_img, uint16_t block_count, u_int16_t inode_count) {
    if(!disk_img) {
        printf("MLFS: Could not open file!\n");
        return;
    }

    INode inodes[inode_count];

    // Defining fs and its superblock
    MLFS fs;
    fs.sb.BlockCount  = block_count;
    fs.sb.BlockSize   = BLOCK_SIZE;
    fs.sb.free_blocks = block_count - 1;
    fs.sb.iNodeCount  = 32;
    fs.sb.EntryCount  = 0;
    fs.inode_table    = inodes;
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

    printf("MLFS: Initialized to disk image!\n");
}

int main(int argc, char** argv) {
    FILE* disk_img = fopen(argv[1], "wb");
    format(disk_img, atoi(argv[2]), atoi(argv[3]));
    fclose(disk_img);
}
