#include "fs_definition.h"
#include "fs.h"
#include <runtime/string.h>
#include <runtime/memory.h>

/* 读扇区 */
s8 * read_sector(u64 sector_id)
{
    return nullptr;
}

/* 写扇区 */
void write_sector(s8 *buffer)
{
    // TODO
}

/* 根据地址位置读一定长度的字节，注意dist必须已经分配了足够的空间 */
void read_sector_by_address(u64 address, u8 *dist, u64 size)
{
    u64 sector_id = address / SECTOR_SIZE;           // 起始扇区
    u64 offset = address - sector_id * SECTOR_SIZE;  // 偏移量    

    while (size > SECTOR_SIZE) {
        // 读出当前对应的扇区的所有数据并写入；
        u8 sector_data[BLOCK_SIZE] = read_sector(sector_id);
        for (u64 i = 0; i < SECTOR_SIZE; ++i) {
            while(offset) {++(sector_data); --offset;}
            (*dist) = (*sector_data);
            ++dist;
            ++sector_data;
        }
        // 然后去除扇区大小字节数
        size -= SECTOR_SIZE;
        ++sector_id;
    }
    // 当剩余字节数小于扇区字节数时……
    u8 sector_data[BLOCK_SIZE] = read_sector(sector_id);
    for (u64 i = 0; i < size; ++i) {
        while(offset) {++(sector_data); --offset;}
        (*dist) = (*sector_data);
        ++dist;
        ++sector_data; 
    }
    return;
}

DirEntry * read_dir_entry(u64 address)
{
    u8 * dist = (u8 *)malloc(sizeof(DirEntry));
    read_sector_by_address(address, dist, sizeof(DirEntry));
    DirEntry * ret = (DirEntry *)malloc(sizeof(DirEntry));
    memcpy(ret, dist, sizeof(DirEntry));
    return ret;
}

/*
*  初始化文件系统
*/
s32 initialize_fs() {
    SuperBlock sb;
    u8 sb_buffer[SUPER_BLOCK_SIZE];
    u64 current_address = 0;


    // STEP 1. 超级块
    // 将超级块的数据读入缓冲
    read_sector_by_address(0, sb_buffer, SUPER_BLOCK_SIZE);  // 暂时假设文件系统的超级块在0号扇区？
    current_address += SECTOR_SIZE;
    // 转换数据类型
    sb = memcpy(&sb, sb_buffer, SUPER_BLOCK_SIZE);
    // 初始化文件系统超级块
    fsInstance = new WN8FileSystem(sb);

    // STEP 2. 读取inode占用情况的bitmap
    // 首先检查超级块中统计整个文件系统的inode数目
    inode_total_num = sb.inode_capacity;
    // 每64个inode的使用情况存在一个unsigned int64中，总共需要：
    fsInodeBitmapLen = (inode_total_num / (sizeof(u64) * 8)) + 1;
    // 分配空间
    fsInodeBitmap = (u64 *)malloc(fsInodeBitmapLen * sizeof(u64));
    // 初始化Inode
    memset(fsInodeBitmap, 0x0000, fsInodeBitmapLen);
    // 从磁盘中读入bitmap：
    u8 * inode_bitmap = (u8 *)malloc(inode_total_num / (sizeof(u8) * 8) + 1);
    read_sector_by_address(current_address, inode_bitmap, inode_total_num / (sizeof(u8) * 8) + 1);
    current_address += SECTOR_SIZE * (inode_total_num / (8 * SECTOR_SIZE));
    if ((inode_total_num % (8 * SECTOR_SIZE)) != 0)
        current_address += SECTOR_SIZE;
    // 将bitmap写入内存：
    memcpy(fsInodeBitmap, inode_bitmap, inode_total_num / (sizeof(u8) * 8) + 1);


    // STEP 3. 读取block占用情况的bitmap
    // 首先检查超级块中统计整个文件系统的block数目
    block_total_num = sb.block_capacity;
    // 每64个inode的使用情况存在一个unsigned int64中，总共需要：
    fsBlockBitmapLen = (block_total_num / (sizeof(u64) * 8)) + 1;
    // 分配空间
    fsBlockBitmap = (u64 *)malloc(fsInodeBitmapLen * sizeof(u64));
    // 初始化Block Bitmap
    memset(fsBlockBitmap, 0x0000, fsInodeBitmapLen);
    // 从磁盘中读入bitmap：
    u8 * block_bitmap = (u8 *)malloc(block_total_num / (sizeof(u8) * 8) + 1);
    read_sector_by_address(current_address, block_bitmap, block_total_num / (sizeof(u8) * 8) + 1);
    current_address += SECTOR_SIZE * (block_total_num / (8 * SECTOR_SIZE));
    if ((block_total_num % (8 * SECTOR_SIZE)) != 0)
        current_address += SECTOR_SIZE;
    // 将bitmap写入内存：
    memcpy(fsBlockBitmap, block_bitmap, block_total_num / (sizeof(u8) * 8) + 1);


    // 读取文件系统根目录
    
    
    
    return 0;
}

