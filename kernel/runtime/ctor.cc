// https://github.com/foreverbell/BadAppleOS/blob/master/kernel/krnl/abi/ctor.cc

#include <runtime/abi.h>

extern u64 INIT_ARRAY_START;
extern u64 CTOR_START;

namespace abi
{

  typedef void (* fn_ptr) (void);

  static fn_ptr * lpfn_inta_ptr = reinterpret_cast<fn_ptr *>(&INIT_ARRAY_START);
  static fn_ptr * lpfn_ctor_ptr = reinterpret_cast<fn_ptr *>(&CTOR_START);

  static void wkctor(fn_ptr * lpfn_ptr) {
    u64 ctor_count = reinterpret_cast<u64>(*lpfn_ptr);
    
    for (int i = 1; i <= ctor_count; ++i) {
      (*(lpfn_ptr + i))();
    }
  }

  void ctors(void) {
    if (lpfn_inta_ptr != NULL) {
      wkctor(lpfn_inta_ptr);
    }
    if (lpfn_ctor_ptr != NULL) {
      wkctor(lpfn_ctor_ptr);
    }
  }

}
