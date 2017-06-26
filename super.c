#include <stdlib.h>
#include <stdio.h>

#include "playdohfs.h"

/*
    General notes:
    -- Many of the calls here returns pointers to structure. That's because in
    Linux VFS, data will be read to struct buffer_head. This structure behaves
    in a manner similar to mmapping a file.
*/

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
        "playdohfs superblock read. Version: %d\n"\
        "Magic number: 0x%08x.\n"\
        "Block size: %d\n"\
        "Free inodes: %d\n"\
        "Max inodes: %d\n"\
        "Free datablocks: %d\n"\
        "Max datablocks: %d\n"\
        "Inode map: 0x%04x\n"\
        "Datablock map: 0x%04x\n",
        sb_disk->version,
        sb_disk->magic,
        sb_disk->blocksize,
        sb_disk->freeinodes,
        sb_disk->maxinodes,
        sb_disk->freeblocks,
        sb_disk->maxblocks,
        sb_disk->inodemap,
        sb_disk->blockmap);

exit:
    return sb_disk;
}

/* Get the inodes table */
playdohfs_inodes_table *get_inodes_table(FILE *fs)
{
    const size_t itables_size = sizeof(playdohfs_inodes_table);
    playdohfs_inodes_table *itable = NULL;
    struct playdohfs_inode *inode = NULL;
    unsigned char i;

    itable = (playdohfs_inodes_table *)malloc(itables_size);
    if(!itable)
    {
        perror("Failed to allocate playdohfs_inodes_table memory");
        goto exit;
    }

    if(fseek(fs, PLAYDOHFS_BLOCKSIZE, SEEK_SET))
    {
        perror("Failed to seek to inodes table");
        goto exit;
    }

    if(fread((void *)itable, 1, itables_size, fs) != itables_size)
    {
        perror("Failed to read inodes table from memory");
        goto exit;
    }

    print_debug("Inodes table content %s", "\n");
    inode = itable->inodes;
    for(i=0; i<PLAYDOHFS_MAX_INODES; i++)
    {
        print_debug(
            "Inode %d: Mode: 0x%04x. blockpos: %d. filesize: %d. filename: %s\n",
            i, inode->mode_t, inode->blockpos, inode->filesize, inode->filename);
        inode++;
    }

exit:
    return itable;
}
