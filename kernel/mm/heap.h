#ifndef _HEAP_H_
#define _HEAP_H_

#include <runtime/types.h>
#include <runtime/memory.h>
#include <mm/malloc.h>
#include <terminal.h>

namespace heap
{

  extern const int NODE_UNUSED;
  extern const int NODE_USED;
  extern const int NODE_SPLIT;
  extern const int NODE_FULL;

  typedef struct buddy_struct {
    u64 level;
    u64 tree[1]; 
  } buddy_t;

  void init();

}

#endif