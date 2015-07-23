/*
×   文件系统基本数据结构定义
×   by JoshOY
×   （才刚学操作系统汇编都没学就写还让不让渣渣活了QAQ）
*/

#ifndef __WN8OS_FS_DEFINITION_H__
#define __WN8OS_FS_DEFINITION_H__

#include "fs_constants.h"

const int IZONENUM = INODE_DIRECT_INDEX_NUM + 2;

/* 超级块结构，参考MINIX */
struct SuperBlock {

/*
    // Linux 0.12的超级块，鉴于比较庞大因此不打算全用

    s16  s_ninodes;       // inode数量
    s16  s_nzones;        // 逻辑块数
    s16  s_imap_blocks;   // inode位图占块数
    s16  s_zmap_blocks;   // 逻辑块位图所占块数
    s16  s_firstdatazone; // 数据区第一个逻辑块块号
    s16  s_log_zone_size; // Log2(磁盘块数/逻辑块数)
    s64  s_max_size;      // 最大文件长度
    s16  s_magic;         // magic value，识别文件系统类型用的

    BufferHead *   s_imap[8];   // inode位图在高速缓冲块的指针数组
    BufferHead *   s_zmap[8];   // 逻辑块位图在高速缓冲块指针数组
    s16            s_dev;       // 超级块所在设备号
    MINode *       s_isup;      // 被安装文件系统根目录的inode指针
    MINode *       s_imount;    // 该文件系统被安装到的inode
    s64            s_time;      // 修改时间
    TaskStruct *   s_wait;      // 等待本超级块的进程指针
    u8             s_lock;      // 锁定标志
    u8             s_rd_only;   // 只读标志
    u8             s_dirt;      // 已被修改（脏）标志
*/

    u64 inode_capacity;   // 可以容纳多少个inode
    u64 inode_allocated;  // 已经分配了多少个inode
    u16 block_size;       // 每个数据块大小
    u64 block_capacity;   // 可以容纳多少个数据块
    u64 block_allocated;  // 已经实际分配了多少个数据块
    u16 magic_value;      // magic value，识别文件系统类型用的
};

const u64 SUPER_BLOCK_SIZE = sizeof(SuperBlock);

/* inode结构，参考MINIX 1.0 */
struct INode {
    
    s16  i_mode;           // 文件类型、权限（rwx）
    s16  i_uid;            // 宿主用户id (user id)
    s64  i_size;           // 文件长度（字节）
    s64  i_mtime;          // 修改时间（since 1970.1.1 0:00:00，单位是秒）
    u8   i_gid;            // 文件宿主的组id (group id)
    u8   i_nlinks;         // 链接数（多少个文件目录项指向该inode）
    s16  i_zone[IZONENUM]; // 文件所占用的盘上逻辑块号组数，0~11为12个直接索引，12为一级间接块号，13为二级间接块号    
    TaskStruct * i_wait;   // 等待该inode的进程    
    s64  i_atime;          // 最后访问时间
    s64  i_ctime;          // inode自身被修改时间
    s16  i_dev;            // inode所在的设备号
    s16  i_num;            // inode号
    s16  i_count;          // inode被引用次数，0时为空闲
    u8   i_lock;           // inode被锁定标志
    u8   i_dirt;           // inode已被修改（脏）标志
    u8   i_pipe;           // inode用作管道标志
    u8   i_mount;          // inode安装了其他文件系统标志
    u8   i_seek;           // 搜索标志（lseek操作时）
    u8   i_update;         // inode已更新标志    
};

/* 文件目录项结构 */
struct DirEntry {
    u16  inode;                // inode号
    u8   name[FILE_NAME_LEN];  // 文件名(256个字符)
};

#endif



