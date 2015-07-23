/**
 * 适用于分配释放页的堆，使用 buddy 算法
 * 来自 https://github.com/cloudwu/buddy
 */

#include <mm/heap.h>

namespace Heap
{

  const int NODE_UNUSED = 0;
  const int NODE_USED   = 1;
  const int NODE_SPLIT  = 2;
  const int NODE_FULL   = 3;

  buddy_t * 
  buddy_new(s64 level) {
    s64 size = 1 << level;
    buddy_t * self = (buddy_t*)malloc(sizeof(buddy_t) + sizeof(uint8_t) * (size * 2 - 2));
    self->level = level;
    memset(self->tree, NODE_UNUSED , size*2-1);
    return self;
  }

  void
  buddy_delete(buddy_t * self) {
    free(self);
  }

  int
  is_pow_of_2(u64 x) {
    return !(x & (x-1));
  }

  u64
  next_pow_of_2(u64 x) {
    if ( is_pow_of_2(x) )
      return x;
    x |= x>>1;
    x |= x>>2;
    x |= x>>4;
    x |= x>>8;
    x |= x>>16;
    return x+1;
  }

  int
  _index_offset(s64 index, s64 level, s64 max_level) {
    return ((index + 1) - (1 << level)) << (max_level - level);
  }

  void 
  _mark_parent(buddy_t * self, s64 index) {
    for (;;) {
      s64 buddy = index - 1 + (index & 1) * 2;
      if (buddy > 0 && (self->tree[buddy] == NODE_USED || self->tree[buddy] == NODE_FULL)) {
        index = (index + 1) / 2 - 1;
        self->tree[index] = NODE_FULL;
      } else {
        return;
      }
    }
  }

  s64 
  buddy_alloc(buddy_t * self , s64 s) {
    s64 size;
    if (s==0) {
      size = 1;
    } else {
      size = (int)next_pow_of_2(s);
    }
    s64 length = 1 << self->level;

    if (size > length)
      return -1;

    s64 index = 0;
    s64 level = 0;

    while (index >= 0) {
      if (size == length) {
        if (self->tree[index] == NODE_UNUSED) {
          self->tree[index] = NODE_USED;
          _mark_parent(self, index);
          return _index_offset(index, level, self->level);
        }
      } else {
        // size < length
        switch (self->tree[index]) {
        case NODE_USED:
        case NODE_FULL:
          break;
        case NODE_UNUSED:
          // split first
          self->tree[index] = NODE_SPLIT;
          self->tree[index*2+1] = NODE_UNUSED;
          self->tree[index*2+2] = NODE_UNUSED;
        default:
          index = index * 2 + 1;
          length /= 2;
          level++;
          continue;
        }
      }
      if (index & 1) {
        ++index;
        continue;
      }
      for (;;) {
        level--;
        length *= 2;
        index = (index+1)/2 -1;
        if (index < 0)
          return -1;
        if (index & 1) {
          ++index;
          break;
        }
      }
    }

    return -1;
  }

  void 
  _combine(buddy_t * self, s64 index) {
    for (;;) {
      s64 buddy = index - 1 + (index & 1) * 2;
      if (buddy < 0 || self->tree[buddy] != NODE_UNUSED) {
        self->tree[index] = NODE_UNUSED;
        while (((index = (index + 1) / 2 - 1) >= 0) &&  self->tree[index] == NODE_FULL){
          self->tree[index] = NODE_SPLIT;
        }
        return;
      }
      index = (index + 1) / 2 - 1;
    }
  }

  void
  buddy_free(buddy_t * self, s64 offset) {
    //assert( offset < (1<< self->level));
    s64 left = 0;
    s64 length = 1 << self->level;
    s64 index = 0;

    for (;;) {
      switch (self->tree[index]) {
      case NODE_USED:
        //assert(offset == left);
        _combine(self, index);
        return;
      case NODE_UNUSED:
        //assert(0);
        return;
      default:
        length /= 2;
        if (offset < left + length) {
          index = index * 2 + 1;
        } else {
          left += length;
          index = index * 2 + 2;
        }
        break;
      }
    }
  }

  int
  buddy_size(buddy_t * self, s64 offset) {
    //assert( offset < (1<< self->level));
    s64 left = 0;
    s64 length = 1 << self->level;
    s64 index = 0;

    for (;;) {
      switch (self->tree[index]) {
      case NODE_USED:
        //assert(offset == left);
        return length;
      case NODE_UNUSED:
        //assert(0);
        return length;
      default:
        length /= 2;
        if (offset < left + length) {
          index = index * 2 + 1;
        } else {
          left += length;
          index = index * 2 + 2;
        }
        break;
      }
    }
  }

  void
  _dump(buddy_t * self, s64 index , s64 level) {
    switch (self->tree[index]) {
    case NODE_UNUSED:
      Terminal::printf("(%d:%d)", _index_offset(index, level, self->level) , 1 << (self->level - level));
      break;
    case NODE_USED:
      Terminal::printf("[%d:%d]", _index_offset(index, level, self->level) , 1 << (self->level - level));
      break;
    case NODE_FULL:
      Terminal::printf("{");
      _dump(self, index * 2 + 1 , level+1);
      _dump(self, index * 2 + 2 , level+1);
      Terminal::printf("}");
      break;
    default:
      Terminal::printf("(");
      _dump(self, index * 2 + 1 , level+1);
      _dump(self, index * 2 + 2 , level+1);
      Terminal::printf(")");
      break;
    }
  }

  void
  buddy_dump(buddy_t * self) {
    _dump(self, 0 , 0);
    Terminal::printf("\n");
  }

  buddy_t * heap;

  void init()
  {
    // TODO
    heap = buddy_new(6);
  }
}