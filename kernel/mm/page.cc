#include <mm/page.h>

#include <terminal.h>

namespace Page
{
  page_map_t PT;   // 每个 PT_entry = 4K，每个 PT = 2M
  page_map_t PD;   // 每个 PD_entry = 2M，每个 PD = 1G
  page_map_t PDP;  // 每个 PDP_entry = 1G，每个 PDP = 512G
  // PML4 暂不支持，目前只映射第0项和第512项（低地址、高地址），每一项可容纳 512GB

  void init(u64 mem_size_KB)
  {
    // 计算最多需要多少 PAGE_TABLE
    // 每个 PAGE_TABLE 是 4KiB，可以表达 512 * 4KiB = 2MiB 内存
    // 所以最多需要 mem_size_byte / 2MiB 个 PAGE_TABLE
    PT.max_n = (mem_size_KB >> 1 >> 10) + 1;
    PT.n_bitmap = (PT.max_n >> 6) + 1;

    // 计算最多需要多少 PAGE_DIRECTORY
    // 每个 PAGE_DIRECTORY 可以容纳 512 个 PAGE_TABLE
    PD.max_n = (PT.max_n >> 9) + 1;
    PD.n_bitmap = (PD.max_n >> 6) + 1;

    // 计算最多需要多少 PDP
    // 每个 PDP 可以容纳 512 个 PD
    PDP.max_n = (PD.max_n >> 9) + 1;
    PDP.n_bitmap = (PDP.max_n >> 6) + 1;

    // 分配位图空间
    PT.bitmap = (u64 *)malloc(PT.n_bitmap * 8);
    PD.bitmap = (u64 *)malloc(PD.n_bitmap * 8);
    PDP.bitmap = (u64 *)malloc(PDP.n_bitmap * 8);

    Terminal::printf("[PAGE] &PT_bitmap: %x, n=%x\n", PT.bitmap, PT.n_bitmap);
    Terminal::printf("[PAGE] &PD_bitmap: %x, n=%x\n", PD.bitmap, PD.n_bitmap);
    Terminal::printf("[PAGE] &PDP_bitmap: %x, n=%x\n", PDP.bitmap, PDP.n_bitmap);
  }

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
   * 获得指定 page_map 类型的闲置块
   */
  u64 get_free_index(page_map_t * map)
  {
    // 从上次分配位置开始搜索
    u64 vector_index = bit_to_index(map->last_alloc_entry);
    u64 index = vector_index << 6;

    while (vector_index < map->n_bitmap) {
      // 按照64位比较进行跳跃
      u64 vec = map->bitmap[vector_index];
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

    return (u64)-1;
  }

  void alloc_frame(page_map_t * map, page_entry_t * entry, int is_kernel, int is_writeable)
  {
    if (entry->frame != 0) {
      return;
    }
    //u64 
  }
}
