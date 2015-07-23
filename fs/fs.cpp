#include "fs_definition.cpp"
#include <runtime/string.h>
#include <runtime/memory.h>

/* 读扇区 */
s8 *read_sector(u64 sector_id)
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


/* 文件系统类 */

class WN8FileSystem {
public:
    WN8FileSystem(SuperBlock sb_init)
    {
        this->superblock = sb_init;
    }

private:
    SuperBlock superblock;

};



/*
*  初始化文件系统
*/
s32 initialize_fs() {
    SuperBlock sb;
    u8 sb_buffer[SUPER_BLOCK_SIZE];

    // 将超级块的数据读入缓冲
    read_sector_by_address(0, sb_buffer, SUPER_BLOCK_SIZE);  // 暂时假设文件系统的超级块在0号扇区？
    
    // 转换数据类型
    sb = memcpy(&sb, sb_buffer, SUPER_BLOCK_SIZE); 

    // 初始化文件系统
    WN8FileSystem wn8fs(sb);
    
}

