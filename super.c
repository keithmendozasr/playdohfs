#include <stdlib.h>
#include <stdio.h>

#include "playdohfs.h"

/*
    Get the superblock information from the filesystem.
    NOTE: This currently leads to a memory leak; however, when this gets
    merged to Linux VFS most of the data will be transfered to the struct 
    super_block data
*/
struct playdohfs_super_block* get_superblock(FILE *fs)
{
    struct playdohfs_super_block *sb_disk = NULL;
    sb_disk = (struct playdohfs_super_block *)malloc(sizeof(struct playdohfs_super_block));
    if(!sb_disk)
    {
        perror("Failed to allocate playdoh_super_block memory");
        goto exit;
    }

    if(fread((void *)sb_disk, 1, playdohfs_sb_size, fs) != playdohfs_sb_size)
    {
        perror("Failed to read playdohfs_super_block data");
        goto exit;
    }
    
    print_debug(
        "playdohfs superblock read. Version: %d. Magic number: 0x%08x. Block size: %d. Free inodes: %d. Max inodes: %d",
        sb_disk->version, sb_disk->magic, sb_disk->blocksize, sb_disk->freeinodes, sb_disk->maxinodes);

exit:
    return sb_disk;
}
