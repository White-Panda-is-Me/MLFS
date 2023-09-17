#include "mlfs.h"

void copy(FILE* Disk, FILE* file, char* fileName) {
    MLFS_Header* bootHeader = malloc(sizeof(MLFS_Header));

    if(fread(bootHeader, sizeof(MLFS_Header), 1, Disk) == 0) {
        printf("Shit! couldn't read fileSector!!!\n");
        free(bootHeader);
        return;
    }

    // check if MLFS+ header is present...
    if(strncmp(bootHeader->mlfs_magic_word, "SFLM", 4) != 0) {
        printf("non MLFS media to format, header not found!");
        free(bootHeader);
        return;
    }

    // read the file data
    fseek(file, 0, SEEK_END);
    uint32_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint32_t neededBlocks = 0;
    if(fileSize % BLOCK_SIZE > 0)
        neededBlocks = (fileSize / BLOCK_SIZE) + 1;
    else
        neededBlocks = (fileSize / BLOCK_SIZE);

    neededBlocks++;          // since we need one sector to write the file informartion
    uint32_t freeBlock = findFreeBlocks(neededBlocks, Disk, bootHeader);

    // add the root entry to the root directory entries struct
    Root_Entries* rootEntries = malloc(sizeof(rootEntries));
    uint32_t freeInode = 0;
    fseek(Disk, (BLOCK_SIZE * 1), SEEK_SET);        // make sure that we are at the second sector
    fread(rootEntries, sizeof(Root_Entries), 1, Disk);
    for (int i = 0;i < 128;i++) {
        if(rootEntries->StartBlockEntries[i] == 0) {          // if found free Inode
            rootEntries->StartBlockEntries[i] = freeBlock;
            freeInode = i;
            break;
        }
    }
    fseek(Disk, (BLOCK_SIZE * 1), SEEK_SET);        // make sure that we are at the second sector
    fwrite(rootEntries, sizeof(Root_Entries), 1, Disk);

    Entry entry;
    entry.Info.file.blockStart = (freeBlock + 1);
    entry.Info.file.fileSize = fileSize;
    entry.Info.file.fileSizeInSector = (neededBlocks - 1);
    entry.inode.inode_num = freeInode;
    entry.inode.CreatedDate = 0;
    entry.inode.CreatedTime = 0;
    entry.inode.flags = MLFS_FLAG_EXECUTABALE | MLFS_FLAG_READABLE | MLFS_FLAG_WRITABLE;
    entry.inode.inode_num = freeInode;
    strncpy(entry.Info.file.name, fileName, 120);

    fseek(Disk, (freeBlock * BLOCK_SIZE), SEEK_SET);
    fwrite(&entry, sizeof(Entry), 1, Disk);

    uint8_t* dataBuffer = malloc(fileSize);
    fread(dataBuffer, fileSize, 1, file);

    fwrite(dataBuffer, fileSize, 1, Disk);

    free(rootEntries);
    free(bootHeader);
    free(dataBuffer);
}

int main(int argc, char** argv) {
    FILE* disk = fopen(argv[1], "rb+");
    FILE* file = fopen(argv[2], "rb");
    if(!file || !disk) {
        printf("couldn't open files!\n");
        fclose(disk);
        fclose(file);
        return 1;
    }

    copy(disk, file, Path2Name(argv[2]));
    printf("File %s has been copied to root dir of %s!\n", argv[2], argv[1]);

    fclose(disk);
    fclose(file);
}