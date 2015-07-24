/*
×   文件系统常数
×   by JoshOY
×   （才刚学操作系统汇编都没学就写还让不让渣渣活了QAQ）
*/

#ifndef __WN8OS_FS_CONSTANT_H__
#define __WN8OS_FS_CONSTANT_H__

/* 一个扇区512 bytes */
const u64 SECTOR_SIZE = 512;

/* 一个inode含12个直接索引 */
const u64 INODE_DIRECT_INDEX_NUM = 12;

/* 文件系统magic数，指明文件系统类型用 */
const u64 magic_s = 0x2333; /* 别吐槽 */

/* 文件名长度值 */
const u64 FILE_NAME_LEN = 256;

/* root inode */
const u64 ROOT_INODE = 1;

/* 一个磁盘块所含的扇区数目（8个） */
const u64 SECTORS_PER_BLOCK = 8;

/* 因此可知一个块的大小（4096 Bytes） */
const u64 BLOCK_SIZE = SECTORS_PER_BLOCK * SECTOR_SIZE;


#endif



