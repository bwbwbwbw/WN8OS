#include "sbrk.h"

#include <arch.h>

extern uintptr_t KERNEL_END;

namespace MM
{

  uintptr_t HEAP_ALLOCATE_ADDRESS = (uintptr_t)&KERNEL_END;

  void * sbrk(ptrdiff_t increment) {
    auto ptr = HEAP_ALLOCATE_ADDRESS;
    
    if (increment < 0) {
      return NULL;
    }
    if (increment > 0) {
      HEAP_ALLOCATE_ADDRESS += increment;
    }
    return (void*)ptr;
  }

}