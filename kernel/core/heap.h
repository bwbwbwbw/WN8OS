#ifndef _HEAP_H_
#define _HEAP_H_

#include <runtime/types.h>
#include <stdlib.h>

namespace Heap
{
void init();
uintptr_t alloc(u64 size, bool align);
uintptr_t alloc_page_aligned(u64 size);

struct buddy;
struct buddy* buddy2_new( int size );
void buddy_destroy( struct buddy2* self );

int buddy_alloc(struct buddy2* self, int size);
void buddy_free(struct buddy2* self, int offset);

int buddy_size(struct buddy2* self, int offset);
void buddy_dump(struct buddy2* self);
}

#endif