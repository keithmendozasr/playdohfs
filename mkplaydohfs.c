#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "playdohfs.h"

int write_datablock(FILE *dest)
{
    struct playdohfs_dir_records data;

    /* Zero out all the data since the 1st block will belong to the empty root
        record. Root's parent inode is itself */
    memset((void *)&data, 0, sizeof(struct playdohfs_dir_records));

    for(unsigned int i=0; i<PLAYDOHFS_MAX_INODES; i++)
    {
        if(fwrite(&data, sizeof(struct playdohfs_dir_records), 1, dest) != 1)
        {
            perror("Failed to write space for data blocks");
            return 1;
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
	FILE *dest;
	struct playdohfs_super_block sb = {
		.magic = PLAYDOHFS_MAGIC_NUM,
		.version = PLAYDOHFS_VERSION,
        .blocksize = PLAYDOHFS_BLOCKSIZE,
        .maxinodes = PLAYDOHFS_MAX_INODES,
		.freeinodes = PLAYDOHFS_MAX_INODES-1,
        .maxblocks = PLAYDOHFS_MAX_INODES,
        .freeblocks = PLAYDOHFS_MAX_INODES-1,
        .inodemap = 1,
        .blockmap = 1
	};

    struct playdohfs_inode *root_node;

    playdohfs_inodes_table inode_table;

    /* Zeroing out the whole array easily gets us each inode marked unused,
        and all other fields zeroed out */
    memset((void *)&inode_table, 0, sizeof(playdohfs_inodes_table));
    
    /* Fillin the root inode */
    root_node = &inode_table.inodes[0];
    strncpy(root_node->filename, "/", 2);
    root_node->blockpos = 0;

    /* Root node is global readable */
    root_node->mode_t = PLAYDOHFS_INODE_FILE | 0x1FF;

	int retVal = EXIT_FAILURE;

	if(argc != 2)
    {
		printf("Usage: mkplaydohfs <loop file>\n");
        return retVal;
    }

    do
    {
		dest = fopen(argv[1], "wb");
		if(!dest)
        {
			perror("Failed to open loop file");
            break;
        }

        if(fwrite(&sb, sizeof(struct playdohfs_super_block), 1, dest) != 1)
        {
            perror("Failed to write superblock");
            break;
        }

        if(fwrite(&inode_table, sizeof(playdohfs_inodes_table), 1, dest) != 1)
        {
            perror("Failed to write inode table");
            break;
        }

        if(write_datablock(dest))
            /* write_datablock would have printed something on screen already */
            break;

        retVal = EXIT_SUCCESS;
        printf("Playdohfs loop file created\n");
    }while(0);

    fclose(dest);

	return retVal;
}

