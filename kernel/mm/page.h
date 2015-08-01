#ifndef _PAGE_H_
#define _PAGE_H_

#include <runtime/types.h>
#include <mm/malloc.h>
#include <mm/heap.h>

namespace Page
{
  typedef struct page_entry_struct
  {
     u32 present    : 1;  // 1=页存在
     u32 rw         : 1;  // 1=页可读写
     u32 user       : 1;  // 1=用户态可访问
     u32 accessed   : 1;  // 1=已访问过
     u32 dirty      : 1;  // 1=已写入过
     u32 unused     : 7;
     u64 frame      : 52; // 地址
  } page_entry_t;

  typedef struct page_table_struct
  {
    page_entry_t entries[512];
  } page_table_t;

  void init(u64 mem_size_KB);

}

#endif
