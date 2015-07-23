#include <heap.h>

extern uintptr_t KERNEL_END;

namespace Heap
{

  uintptr_t HEAP_ALLOCATE_ADDRESS;
  
  void init()
  {
    HEAP_ALLOCATE_ADDRESS = (uintptr_t)&KERNEL_END;
  }

  uintptr_t alloc(u64 size, bool align)
  {
    // 4KB 对齐
    if (align == 1 && (HEAP_ALLOCATE_ADDRESS & 0xFFFFF000))
    {
      HEAP_ALLOCATE_ADDRESS &= 0xFFFFF000;
      HEAP_ALLOCATE_ADDRESS += 0x1000;
    }
    uintptr_t ret = HEAP_ALLOCATE_ADDRESS;
    HEAP_ALLOCATE_ADDRESS += size;
    return ret;
  }

  uintptr_t alloc_page_aligned(u64 size)
  {
    return alloc(size, true);
  }

}