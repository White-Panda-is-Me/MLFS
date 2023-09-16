#include "mlfs.h"

void format(FILE* Disk, FILE* Boot, uint32_t sectorNum) {
    uint8_t* BootCode = malloc(512);

    if(fread(BootCode, BLOCK_SIZE, 1, Boot) == 0) {
        printf("Shit! couldn't read BootSector!!!\n");
        fclose(Disk);
        fclose(Boot);
        free(BootCode);
    }

    MLFS_Header* BootHeader = malloc(sizeof(MLFS_Header));
    memcpy(BootHeader, BootCode, sizeof(MLFS_Header));

    // check if MLFS+ header is present...
    if(strncmp(BootHeader->mlfs_magic_word, "SFLM", 4) != 0) {
        printf("non MLFS media to format, header not found!");
        fclose(Disk);
        fclose(Boot);
        free(BootCode);
        free(BootHeader);
    }

    // write <sectorNum> sectors to the disk (i.e make a disk with the specified size)
    uint64_t zero = 0;
    for (uint64_t i = 0;i < sectorNum * 64;i++) {
        fwrite(&zero, 8, 1, Disk);
    }

    // copy the boot record to the first sector of the disk
    fseek(Disk, 0, SEEK_SET);
    fwrite(BootCode, BLOCK_SIZE, 1, Disk);

    // the root dir entries section goes here
    Root_Entries* rootEntries = malloc(sizeof(rootEntries));
    fseek(Disk, (BLOCK_SIZE * 1), SEEK_SET);        // make sure that we are at the second sector
    fwrite(rootEntries, sizeof(Root_Entries), 1, Disk);

    // now we have to write the Inode table into the disk
    fseek(Disk, (BLOCK_SIZE * 2), SEEK_SET);        // make sure that we are at the third sector
    INode* inode_table = malloc((sizeof(INode) * 512));
    fwrite(inode_table, (sizeof(INode) * 512), 1, Disk);

    fclose(Disk);
    fclose(Boot);
    free(inode_table);
    free(BootHeader);
    free(BootCode);
    free(rootEntries);
}

int main(int argc, char** argv) {
    FILE* disk = fopen(argv[1], "rb+");
    FILE* boot = fopen(argv[2], "rb");
    if(!boot || !disk) {
        printf("couldn't open files!\n");
        fclose(disk);
        fclose(boot);
        return 1;
    }
    format(disk, boot, atoi(argv[3]));
    printf("Disk has been formatted!\n");

    fclose(disk);
    fclose(boot);
}