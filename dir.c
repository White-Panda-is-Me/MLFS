#include "mlfs.h"

void mkdir(FILE* disk, char* name, char* dir) {
    
}

int main(int argc, char** argv) {
    FILE* img = fopen(argv[1], "rb+");
    if(!img) {
        printf("MLFS: FILE NOT FOUND: build/disk.iso");
        return 1;
    }

    mkdir(img, argv[2], argv[3]);

    fclose(img);
}