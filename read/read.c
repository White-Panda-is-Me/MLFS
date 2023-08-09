#include "../format/format.h"
#include <stdlib.h>

int read(FILE* img, const char* filename) {
    MLFS* FS;
    Entry* entry;
    char* buffer;
    entry = malloc(sizeof(Entry));
    FS = malloc(sizeof(MLFS));
    fread(FS, sizeof(MLFS), 1, img);

    for(int i = 0;i < FS->sb.iNodeCount;i++) {
        fseek(img, (BLOCK_SIZE * i), SEEK_SET);
        fread(entry, sizeof(Entry), 1, img);
        if(strcmp(entry->name, filename) == 0) {
            buffer = malloc(FS->inode_table[(FS->sb.iNodeCount - i)].size);
            int bytes_read = fread(buffer, sizeof(char), FS->inode_table[(FS->sb.iNodeCount - i)].size, img);
            printf("%s\n", buffer);
            free(buffer);
            free(entry);
            free(FS);
            return bytes_read;
        }
    }
    printf("MLFS: File not found: %s", filename);
    return 0;
}

int main(int argc, char** argv) {
    FILE* img = fopen("/dev/disk2", "rb");
    if(!img) {
        printf("iso not found!");
        return 1;
    }
    read(img, "test.txt");
    fclose(img);
}