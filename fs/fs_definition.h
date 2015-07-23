/*
×   文件系统基本数据结构定义
×   by JoshOY
×   （才刚学操作系统汇编都没学就写还让不让渣渣活了QAQ）
*/

#ifndef __WN8OS_FS_DEFINITION_H__
#define __WN8OS_FS_DEFINITION_H__

#include "fs_constants.h"

const int IZONENUM = INODE_DIRECT_INDEX_NUM + 2;

/* 超级块结构 */
struct SuperBlock {
    u64 inode_capacity;   // 可以容纳多少个inode
    u64 inode_allocated;  // 已经分配了多少个inode
    u16 block_size;       // 每个数据块大小
    u64 block_capacity;   // 可以容纳多少个数据块
    u64 block_allocated;  // 已经实际分配了多少个数据块
    u16 magic_value;      // magic value，识别文件系统类型用的
    u64 root_inode_addr;  // 文件系统根目录的地址
};

const u64 SUPER_BLOCK_SIZE = sizeof(SuperBlock);

/* inode结构*/
struct INode {
    u64 inode_index;       // inode的编号
    u64 direct_index[12];  // 12个直接索引（block索引）
    u64 lv1_index;         // 一级索引
    u64 lv2_index;         // 二级索引
};

/* 文件目录项结构 */
struct DirEntry {
    u64 inode_index;          // inode号
    u8  name[FILE_NAME_LEN];  // 文件名(不超过256个字符)
};

#endif



