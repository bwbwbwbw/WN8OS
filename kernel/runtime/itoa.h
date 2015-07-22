#ifndef _ITOA_H_
#define _ITOA_H_

#include <runtime/types.h>

extern "C" {

char* itoa(u64 num, char* str, int base);

}

#endif