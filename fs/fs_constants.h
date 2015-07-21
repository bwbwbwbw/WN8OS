/*
×   文件系统常数
×   by JoshOY
×   （才刚学操作系统汇编都没学就写还让不让渣渣活了QAQ）
*/

#ifndef __WN8OS_FS_CONSTANT_H__
#define __WN8OS_FS_CONSTANT_H__

/* 一个扇区512 bytes */
const int SECTOR_SIZE = 512;

/* 一个inode含12个直接索引 */
const int INODE_DIRECT_INDEX_NUM = 12;

/* 文件系统magic数，指明文件系统类型用 */
const int magic_s = 0x2333; /* 别吐槽 */

/* 文件名长度值 */
const int FILE_NAME_LEN = 256;

/* root inode */
const int ROOT_INODE = 1;

#endif



