#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <runtime/types.h>

extern "C" {

void *memchr(const void *, int, size_t);
int memcmp(const void *, const void *, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);

}

#endif
