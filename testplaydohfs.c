#include <stdio.h>
#include <stdlib.h>

#include "playdohfs.h"

int main(int argc, char **argv)
{
    FILE *fs;
    struct playdohfs_super_block *sb=NULL;
    playdohfs_inodes_table *itable = NULL;

    if(argc != 2)
    {
        printf("Usage: testplaydohfs <file system object>\n");
        return 1;
    }

    if((fs = fopen(argv[1], "rb")) == NULL)
    {
        perror("Failed to open file system object");
        return 1;
    }

    do
    {
        sb = get_superblock(fs);
        if(!sb)
            break;

        itable = get_inodes_table(fs);

    }while(0);

    if(sb)
        free(sb);
    if(itable)
        free(itable);

    fclose(fs);
    return 0;
}
