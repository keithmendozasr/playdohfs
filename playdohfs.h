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
    char padding[4096-(sizeof(uint32_t)*5)];
};
const size_t playdohfs_sb_size = sizeof(struct playdohfs_super_block);

/*
    Structure for 1 inode
*/
#define PLAYDOHFS_INODE_FREE    0x0
#define PLAYDOHFS_INODE_DIR     0x1
#define PLAYDOHFS_INODE_FILE    0x2
struct __attribute__ ((__packed__)) playdohfs_inode
{
    char filename[PLAYDOHFS_MAX_FILENAME+1];
    uint8_t type;
    uint32_t filesize;
    uint32_t blockpos;
};

/* 
    Making max number of inodes to 40 since each playdohfs_inode struct is 102
    bytes wide. Making an array of 40 playdohfs_inode will fit in 4096-byte block
    that'll fit well with sb_bread
*/
#define PLAYDOHFS_MAX_INODES 40u
typedef struct __attribute__ ((__packed__)) playdohfs_inode_table
{
    struct playdohfs_inode inodes[PLAYDOHFS_MAX_INODES];
} playdohfs_inodes_table;

/* Use to print "debug" message. For now just use printf. When this is merged to
    Linux VFS, pr_debug will be used */
#define print_debug(fmt, ...) printf(fmt, __VA_ARGS__)

#endif
