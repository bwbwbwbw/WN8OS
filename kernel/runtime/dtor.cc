// https://github.com/foreverbell/BadAppleOS/blob/master/kernel/krnl/abi/dtor.cc

#include <runtime/types.h>
#include <runtime/abi.h>

typedef void (* fn_ptr) (void *);

namespace ABI {

  static int cxa_atexit(fn_ptr lpfn, void *pobj, void * /* dso_handle */) {
    return 0;
  }

  static void cxa_finalize(fn_ptr lpfn) {
    return;
  }

}

/* exports */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void *__dso_handle = 0; 

int __cxa_atexit(fn_ptr lpfn, void *pobj, void *dso_handle) {
  return ABI::cxa_atexit(lpfn, pobj, dso_handle);
}

void __cxa_finalize(fn_ptr lpfn) {
  ABI::cxa_finalize(lpfn);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

namespace ABI {
  
void dtors(void) {
  __cxa_finalize(NULL);
}

} /* ABI */