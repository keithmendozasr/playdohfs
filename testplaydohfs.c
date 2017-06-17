#include <stdio.h>

extern struct playdohfs_super_block* get_superblock(FILE *);

int main(int argc, char **argv)
{
    FILE *fs;

    if(argc != 2)
    {
        printf("Usage: dumpplaydohfs <file system object>\n");
        return 1;
    }

    if((fs = fopen(argv[1], "rb")) == NULL)
    {
        perror("Failed to open file system object");
        return 1;
    }

    (void)get_superblock(fs);

    fclose(fs);
    return 0;
}
