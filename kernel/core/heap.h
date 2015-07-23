#ifndef _HEAP_H_
#define _HEAP_H_

#include <runtime/types.h>

namespace Heap
{
  void init();
  uintptr_t alloc(u64 size, bool align);
  uintptr_t alloc_page_aligned(u64 size);
}

#endif