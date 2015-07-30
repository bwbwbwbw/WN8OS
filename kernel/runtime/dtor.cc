// https://github.com/foreverbell/BadAppleOS/blob/master/kernel/krnl/abi/dtor.cc

#include <runtime/abi.h>

extern "C" {

typedef void (* fn_ptr) (void *);

int __cxa_atexit(fn_ptr lpfn, void *pobj, void *dso_handle)
{
  return 0;
}

void __cxa_finalize(fn_ptr lpfn)
{
  return;
}

}

namespace ABI
{
  
  void dtors(void)
  {
    __cxa_finalize(NULL);
  }

} /* ABI */