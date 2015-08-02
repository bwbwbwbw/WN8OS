#ifndef _MM_SBRK_H
#define _MM_SBRK_H

#include <runtime/types.h>

namespace mm
{

  void * sbrk(ptrdiff_t increment);

}

#endif