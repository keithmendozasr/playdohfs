#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "playdohfs.h"

int main(int argc, char **argv)
{
	FILE *dest;
	struct playdohfs_super_block sb = {
		.magic = PLAYDOHFS_MAGIC_NUM,
		.version = PLAYDOHFS_VERSION,
        .blocksize = PLAYDOHFS_BLOCKSIZE,
		.freeinodes = PLAYDOHFS_MAX_INODES,
        .maxinodes = PLAYDOHFS_MAX_INODES
	};

    playdohfs_inodes_table inode_table;

    /* Zeroing out the whole array easily gets us each inode marked unused,
        and all other fields zeroed out */
    memset((void *)&inode_table, 0, sizeof(playdohfs_inodes_table));

	int retVal = EXIT_FAILURE;

	if(argc != 2)
		printf("Usage: mkplaydohfs <loop file>\n");
	else
	{
		dest = fopen(argv[1], "wb");
		if(!dest)
			perror("Failed to open loop file");
		else
		{
			if(fwrite(&sb, sizeof(struct playdohfs_super_block), 1, dest) != 1)
				perror("Failed to write superblock");
			else
			{
                if(fwrite(&inode_table, sizeof(playdohfs_inodes_table), 1, dest) != 1)
                    perror("Failed to write inode table");
                else
                {
                    /* Just using the superblock to fill in the datablock area
                        for convenience */
                    retVal = EXIT_SUCCESS;
                    for(int i=0; i<40; i++)
                    {
                        if(fwrite(&sb, sizeof(struct playdohfs_super_block), 1, dest) != 1)
                        {
                            perror("Failed to write space for data blocks");
                            retVal = EXIT_FAILURE;
                        }
                    }
                    if(retVal == EXIT_SUCCESS)
                        printf("Playdohfs loop file created\n");
                }
			}
		}
	}

	return retVal;
}

