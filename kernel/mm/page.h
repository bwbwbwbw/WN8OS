#ifndef _PAGE_H_
#define _PAGE_H_

#include <runtime/types.h>
#include <mm/malloc.h>
#include <mm/heap.h>

namespace Page
{
  struct page_entry_struct
  {
     u32 present    : 1;  // 1=页存在
     u32 rw         : 1;  // 1=页可读写
     u32 user       : 1;  // 1=用户态可访问
     u32 accessed   : 1;  // 1=已访问过
     u32 dirty      : 1;  // 1=已写入过
     u32 unused     : 7;
     u64 frame      : 52; // 地址
  };
  typedef struct page_entry_struct page_entry_t;

  struct page_table_struct
  {
    page_entry_t entries[512];
  };
  typedef struct page_table_struct page_table_t;

  struct page_map_struct
  {
    u64 max_n;    // 最大 entry 数量
    u64 * bitmap; // 分配的位图指针，每个位图向量（u64）可以表示 64 个 entry
    u64 n_bitmap; // 位图向量个数
    u64 last_alloc_entry; // 最后一次分配索引号（仅适合最后一级页表）
  };
  typedef struct page_map_struct page_map_t;

  void init(u64 mem_size_KB);

}

#endif