/*
×   文件系统基本数据结构定义
×   by JoshOY
×   （才刚学操作系统汇编都没学就写还让不让渣渣活了QAQ）
*/

#ifndef __WN8OS_FS_DEFINITION_H__
#define __WN8OS_FS_DEFINITION_H__

#include "fs_constants.h"
#include <runtime/string.h>
#include <runtime/memory.h>

const u64 IZONENUM = INODE_DIRECT_INDEX_NUM + 2;
const u64 _UNALLOC  = 0xFFFFFFFFFFFFFFFF;
const u64 _MAX_LV1_INDEX_BLOCKS = BLOCK_SIZE / sizeof(u64);  // 一级索引块最多塞的下多少个Block的索引


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
struct _INode {
    u64 sizeByte;          //　文件大小（字节）
    u64 accessTime;        // 最后访问时间戳
    u64 modifyTime;        // 最后修改时间戳
    u64 creationTime;      // 创建时间戳
    u64 linkCouunt;        // 链接数
    u8  flags;             // 属性，最低位0代表是文件，最低位1代表是目录
    u32 ownerId;           // 所有者用户ID
    u64 inode_index;       // inode的编号
    u64 direct_index[12];  // 12个直接索引（block索引）
    u64 lv1_index;         // 一级索引
    u64 lv2_index;         // 二级索引
    u64 used_blocks;       // 总共使用的块数
};

/* Block结构 */
struct _Block {
    u8 bytes[BLOCK_SIZE];
};

/* ====================================== *
*                 Classes                 *
*  ====================================== */

/* 文件系统类 */
class WN8FileSystem {
public:
    WN8FileSystem(SuperBlock sb_init):sb(sb_init){}
    static u64 fs_alloc_new_block();
    static u64 fs_alloc_new_inode();
private:
    SuperBlock sb;
};


/* Block类 */
class Block {
public:
    Block(u8 * init_bytes);

    void write_bytes(u8 * buffer, u64 buflen);
    void write_append(u8 * buffer, u64 buflen);

private:
    _Block * data_block;
    u64      block_used_length;
};

/* inode操作类 */
class INode {
public:
    INode(u64 inode_idx);
    void write(u8 * buffer);
    void write_append(u8 * buffer, u64 buffer_len);
    void alloc_new_block(u64 new_block_idx);
private:
    struct _INode *data;
};


/* ====================================== *
*           Functions definitions         *
*  ====================================== */

// Class Block

Block::Block(u8 * init_bytes)
{
    this->data_block = new _Block();
    memset(this->data_block->bytes, 0, BLOCK_SIZE);
    return;
}

void Block::write_bytes(u8 * buffer, u64 buflen)
{
    if (buflen > BLOCK_SIZE) {
        return;
    }

    this->block_used_length = buflen;
    memcpy(buffer, this->data_block->bytes, buflen);
    
    return;
}

void Block::write_append(u8 * buffer, u64 buflen)
{
    if (buflen + this->block_used_length > BLOCK_SIZE) {
        return;
    }

    memcpy(buffer, &(this->data_block->bytes[block_used_length]), buflen); 
    
    this->block_used_length += buflen;
    return;
}

// Class INode

INode::INode(u64 inode_idx)
{
    this->data = new _INode();
    this->data->sizeByte = 0;
    this->data->accessTime = 0;
    this->data->modifyTime = 0;
    this->data->creationTime = 0;
    this->data->linkCount = 0;
    this->data->flags = 0x0000;
    this->data->ownerId = 0;
    this->data->inode_index = inode_idx;
    this->data->direct_index = new u64[12];
    for (u64 i = 0; i < 12; i++) {
        direct_index[i] = _UNALLOC;
    }
    this->data->lv1_index = _UNALLOC;
    this->data->lv2_index = _UNALLOC;
    this->used_blocks = 0;
}

void INode::alloc_new_block(u64 new_block_idx);
{
    auto next_block_index = this->data->used_blocks + 1;
    if (next_block_index < 12) {
        this->data->direct_index[next_block_index] = new_block_idx;
        return;
    }
    else if (next_block_index == 12) {
        this->data->lv1_index = WN8FileSystem::fs_alloc_new_block();
    }
    else if ((next_block_index > 12) && (next_block_index <= _MAX_LV1_INDEX_BLOCKS)) {
        // TODO：当已经有了一个一级索引块时应该如何添加
    }
    else if (next_block_index == _MAX_LV1_INDEX_BLOCKS + 1) {
        // TODO
    }
    else {
        // TODO
    }
}

#endif
