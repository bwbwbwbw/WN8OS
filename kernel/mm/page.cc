#include "page.h"

#include <arch.h>
#include <mm/malloc.h>
#include <runtime/memory.h>

extern void * PML4_BASE;

namespace page
{
  /** 一共有多少帧 */
  u64 n_frame;

  /** 有多少位图向量 */
  u64 n_frame_bitmap;

  /** 存储位图向量 */
  u64 * frame_bitmap;

  /** 最后一次分配的是哪一帧 */
  u64 last_alloc_index;

  /** PML4 内存，线性地址 */
  page_table_t * PML4;

  /** 由于我们仅仅映射 PML4 第 0 项和第 256 项，所以直接存储相应的 PDP，可以方便后续操作，这里是线性地址 */
  page_table_t * PDP_user, * PDP_kernel;

  /**
   * 给定一个位图索引，返回其向量位置
   */
  inline u64 bit_to_index(u64 bitIndex)
  {
    return bitIndex >> 6;
  }

  /**
   * 给定一个位图索引，返回其在向量中的偏移
   */
  inline u64 bit_to_index_offset(u64 bitIndex)
  {
    return bitIndex & 63;
  }

  /**
   * 获得可用帧
   */
  u64 get_free_frame()
  {
    // 从上次分配位置开始搜索
    auto vector_index = bit_to_index(last_alloc_index);
    auto index = vector_index << 6;

    while (vector_index < n_frame_bitmap) {
      // 按照64位比较进行跳跃
      u64 vec = frame_bitmap[vector_index];
      if (vec == 0xFFFFFFFFFFFFFFFF) {
        index += 64;
        continue;
      }
      if (vec & 0xFFFFFFFF == 0xFFFFFFFF) {
        index += 32;
        vec >>= 32;
      }
      if (vec & 0xFFFF == 0xFFFF) {
        index += 16;
        vec >>= 16;
      }
      if (vec & 0xFF == 0xFF) {
        index += 8;
        vec >>= 8;
      }
      if (vec & 0xF == 0xF) {
        index += 4;
        vec >>= 4;
      }
      if (vec & 0x3 == 0x3) {
        index += 2;
        vec >>= 2;
      }
      if (vec & 0x1 == 0x1) {
        index += 1;
        vec >>= 1;
      }
      return index;
    }

    return static_cast<u64>(-1);
  }

  inline page_entry_t make_empty_page_table_entry()
  {
    page_entry_t entry;
    entry.present = 0;
    return entry;
  }

  inline page_entry_t make_page_table_entry(u32 rw, u32 user, uintptr_t addr)
  {
    page_entry_t entry;
    entry.present = 1;
    entry.rw = rw;
    entry.user = user;
    entry.accessed = 0;
    entry.dirty = 0;
    entry.unused = 0;
    entry.frame = addr >> 12;
    return entry;
  }

  void init(u64 mem_size_KB)
  {
    // 初始化页帧的位图
    n_frame = mem_size_KB >> 2;
    n_frame_bitmap = bit_to_index(n_frame) + 1;
    frame_bitmap = reinterpret_cast<u64*>(malloc(n_frame_bitmap * sizeof(u64)));
    for (int i = 0; i < n_frame_bitmap; ++i) {
      frame_bitmap[i] = 0;
    }
    last_alloc_index = 0;

    // 希望建立的线性地址到物理地址映射关系为：
    //         线性地址                                       => 物理地址
    // ==========================================================================
    // 内核空间 KERNEL_VMA_BASE ~ KERNEL_VMA_BASE + mem_size  => 0 ~ mem_size
    // 用户空间 0 ~ KERNEL_VMA_BASE                           => 动态分配帧
    
    // PML4 复用临时建立的 PML4
    PML4 = reinterpret_cast<page_table_t *>(reinterpret_cast<uintptr_t>(&PML4_BASE) + KERNEL_VMA_BASE);

    // 建立内核 PDP
    PDP_kernel = reinterpret_cast<page_table_t *>(memalign(4096, 4096));  // 4096 aligned

    uintptr_t addr = 0, addr_max = mem_size_KB << 10;

    // 填充内核 PDP 下每一项 PD
    int page_dir_index = 0;
    for (; page_dir_index < 512 && addr <= addr_max; ++page_dir_index) {
      page_table_t * PD = reinterpret_cast<page_table_t *>(memalign(4096, 4096));
      // rw = 1, user = 0
      PDP_kernel->entries[page_dir_index] = make_page_table_entry(1, 0, reinterpret_cast<uintptr_t>(PD) - KERNEL_VMA_BASE);

      // 填充 PD 下每一项 PT
      int page_table_index = 0;
      for (; page_table_index < 512 && addr <= addr_max; ++page_table_index) {
        page_table_t * PT = reinterpret_cast<page_table_t *>(memalign(4096, 4096));
        // rw = 1, user = 0
        PD->entries[page_table_index] = make_page_table_entry(1, 0, reinterpret_cast<uintptr_t>(PT) - KERNEL_VMA_BASE);

        // 填充 PT 下每一项 PageEntry
        int page_index = 0;
        for (; page_index < 512 && addr <= addr_max; ++page_index) {
          // rw = 1, user = 0
          PT->entries[page_index] = make_page_table_entry(1, 0, addr);
          addr += 4096;
        }

        // 剩余的每一项 PageEntry 填充 0
        for (; page_index < 512; ++page_index) {
          PT->entries[page_index] = make_empty_page_table_entry();
        }
      }

      // 剩余的每一项 PageTableEntry 填充 0
      for (; page_table_index < 512; ++page_table_index) {
        PD->entries[page_table_index] = make_empty_page_table_entry();
      }
    }

    // 剩余的每一项 PageDirectoryEntry 填充 0
    for (; page_dir_index < 512; ++page_dir_index) {
      PDP_kernel->entries[page_dir_index] = make_empty_page_table_entry();
    }

    PML4->entries[0] = make_empty_page_table_entry();
    PML4->entries[256] = make_page_table_entry(1, 0, reinterpret_cast<uintptr_t>(PDP_kernel) - KERNEL_VMA_BASE);
  }

}
