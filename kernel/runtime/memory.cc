/**
 * 基本内存处理，来自 glibc / Apple libc
 */

#include <runtime/memory.h>

extern "C" {

void *
memchr(const void * s, int c, size_t n)
{
  if (n != 0) {
    const unsigned char *p = (const unsigned char *) s;

    do {
      if (*p++ == c)
        return ((void *)(p - 1));
    } while (--n != 0);
  }
  return NULL;
}

int
memcmp(const void * s1, const void * s2, size_t n)
{
    int v = 0;
    char *p1 = (char *)s1;
    char *p2 = (char *)s2;

    while(n-- > 0 && v == 0) {
        v = *(p1++) - *(p2++);
    }

    return v;
}

/*
 * sizeof(word) MUST BE A POWER OF TWO
 * SO THAT wmask BELOW IS ALL ONES
 */
typedef int word;   /* "word" used for optimal copy speed */

#define wsize sizeof(word)
#define wmask (wsize - 1)

/*
 * Copy a block of memory, handling overlap.
 * This is the routine that actually implements
 * (the portable versions of) bcopy, memcpy, and memmove.
 */
void * memcpy(void *dst0, const void *src0, size_t length)
{
  char *dst = (char *)dst0;
  const char *src = (const char *)src0;
  size_t t;
  
  if (length == 0 || dst == src)    /* nothing to do */
    goto done;
  
  /*
   * Macros: loop-t-times; and loop-t-times, t>0
   */
#define TLOOP(s) if (t) TLOOP1(s)
#define TLOOP1(s) do { s; } while (--t)
  
  if ((unsigned long)dst < (unsigned long)src) {
    /*
     * Copy forward.
     */
    t = (uintptr_t)src; /* only need low bits */
    if ((t | (uintptr_t)dst) & wmask) {
      /*
       * Try to align operands.  This cannot be done
       * unless the low bits match.
       */
      if ((t ^ (uintptr_t)dst) & wmask || length < wsize)
        t = length;
      else
        t = wsize - (t & wmask);
      length -= t;
      TLOOP1(*dst++ = *src++);
    }
    /*
     * Copy whole words, then mop up any trailing bytes.
     */
    t = length / wsize;
    TLOOP(*(word *)dst = *(word *)src; src += wsize; dst += wsize);
    t = length & wmask;
    TLOOP(*dst++ = *src++);
  } else {
    /*
     * Copy backwards.  Otherwise essentially the same.
     * Alignment works as before, except that it takes
     * (t&wmask) bytes to align, not wsize-(t&wmask).
     */
    src += length;
    dst += length;
    t = (uintptr_t)src;
    if ((t | (uintptr_t)dst) & wmask) {
      if ((t ^ (uintptr_t)dst) & wmask || length <= wsize)
        t = length;
      else
        t &= wmask;
      length -= t;
      TLOOP1(*--dst = *--src);
    }
    t = length / wsize;
    TLOOP(src -= wsize; dst -= wsize; *(word *)dst = *(word *)src);
    t = length & wmask;
    TLOOP(*--dst = *--src);
  }
done:
  return (dst0);
}

void *
memmove(void *s1, const void *s2, size_t n)
{
  return memcpy(s1, s2, n);
}

}