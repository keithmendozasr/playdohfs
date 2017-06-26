#ifndef PLAYDOHFS_SUPER_H
#define PLAYDOHFS_SUPER_H

#include <stdint.h>

#define PLAYDOHFS_MAGIC_NUM 0x00729364
#define PLAYDOHFS_BLOCKSIZE 4096u
#define PLAYDOHFS_VERSION 1u
#define PLAYDOHFS_MAX_FILENAME 255u

/* 
    To make it simple, I'm going
    to make this superblock structure that'll live in the "disk" a 4096-byte
    structure. That way, I can just use sb_read without having to set the
    blocksize field of struct super_block. Also, marking this struct as packed
    to make sure that it doesn't get padded	by the compiler
*/
struct __attribute__ ((__packed__)) playdohfs_super_block 
{
    uint32_t magic;
    uint32_t version;
    uint32_t blocksize;
	uint32_t freeinodes;
    uint32_t maxinodes;
    uint32_t freeblocks;
    uint32_t maxblocks;
    uint16_t inodemap;
    uint16_t blockmap;

    /* The 2 uint16_t are being counted as 1 uint32_t */
    char padding[4096-(sizeof(uint32_t)*8)];
};
static const size_t playdohfs_sb_size = sizeof(struct playdohfs_super_block);

/*
    Structure for 1 inode
*/
#define PLAYDOHFS_INODE_FREE    0x0000
#define PLAYDOHFS_INODE_DIR     0x0100
#define PLAYDOHFS_INODE_FILE    0x0200
struct __attribute__ ((__packed__)) playdohfs_inode
{
    char filename[PLAYDOHFS_MAX_FILENAME+1];
    uint32_t filesize;

    /* For now only put files in 1 block, which will limit the size we can store
        to 4096 */
    uint32_t blockpos;

     /* Mode and inode type. 
        Bits 0-7 is for the file type. Bits 8-15 is for the permissions. */
     uint16_t mode_t;
};

/* 
    Making max number of inodes to 12 since each playdohfs_inode struct is 336
    bytes wide. Making an array of 12 playdohfs_inode will fit in 4096-byte block
    that'll fit well with sb_bread
*/
#define PLAYDOHFS_MAX_INODES 12u
typedef struct __attribute__ ((__packed__)) playdohfs_inodes_table
{
    struct playdohfs_inode inodes[PLAYDOHFS_MAX_INODES];
} playdohfs_inodes_table;

/* This will be the directory's block data content */
struct __attribute__ ((__packed__)) playdohfs_dir_records
{
    uint8_t child_count;

    /* Worst-case scenario is all files are in the same directory */
    uint32_t content_inodes[PLAYDOHFS_MAX_INODES];

    /* Parent directory inode
        TODO: Re-evaluate if this is necessary in Linux VFS
    */
    uint32_t parent_inode;

    char padding[4096 - (sizeof(uint8_t)) - (sizeof(uint32_t) * (PLAYDOHFS_MAX_INODES + 1))];
};

/* Use to print "debug" message. For now just use printf. When this is merged to
    Linux VFS, pr_debug will be used */
#define print_debug(fmt, ...) printf(fmt, __VA_ARGS__)

/* Function definitions */
extern struct playdohfs_super_block* get_superblock(FILE *fs);
extern struct playdohfs_inodes_table* get_inodes_table(FILE *fs);

#endif
