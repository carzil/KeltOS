#include "kernel/alloc.h"
#include "kernel/types.h"
#include "kernel/printk.h"
#include "kernel/defs.h"
#include "kernel/memory.h"

extern u32 _edata;

#define NULL ((void*)0)

struct chunk {
    u32 size;
    u32 flags;
    struct chunk* next;

    u8 data[0];
};

static struct chunk* head_chunk = NULL;

struct chunk* chunk_left(void* chunk)
{
    chunk -= sizeof(u32);
    chunk -= *(u32*)chunk;
    chunk -= sizeof(struct chunk);
    return chunk;
}

struct chunk* chunk_right(void* chunk)
{
    chunk += ((struct chunk*)chunk)->size;
    chunk += sizeof(struct chunk);
    chunk += sizeof(u32);
    return chunk;
}

void chunk_set_size(struct chunk* chunk, u32 size)
{
    chunk->size = size;
    *(u32*)(chunk->data + size) = size;
}

void mm_init()
{
    u32 size;

    head_chunk = data_brk;
    head_chunk->flags = CHUNK_FREE;
    size = SRAM_SIZE - STACK_SIZE - sizeof(struct chunk) - sizeof(u32);
    head_chunk->next = NULL;
    head_chunk->size = size;
    chunk_set_size(head_chunk, size);
}

/*
 * Splits given chunk into two chunks of size (ptr->size - size) and (size).
 */
struct chunk* chunk_split(struct chunk* ptr, u32 size)
{
    struct chunk* new_chunk;

    ptr->size -= sizeof(struct chunk) + size + sizeof(u32);
    new_chunk = ((void*)ptr) + sizeof(struct chunk) + ptr->size + sizeof(u32);
    new_chunk->size = size;
    new_chunk->flags = ptr->flags;
    new_chunk->next = ptr->next;

    ptr->next = new_chunk;
    chunk_set_size(ptr, ptr->size);
    chunk_set_size(new_chunk, new_chunk->size);
    return new_chunk;
}

void chunk_merge(struct chunk* left, struct chunk* right)
{
    left->size += sizeof(struct chunk) + right->size + sizeof(u32);
    chunk_set_size(left, left->size);
}

void* kalloc(u32 size)
{
    struct chunk* ptr;
    struct chunk* new_chunk;

    if (size & 3) {
        /* align up size to 4 bytes */
        size += 4 - (size & 3);
    }

    // TODO: disable all irqs and suspend scheduler here

    ptr = head_chunk;
    while (ptr && ptr->size < size) {
        ptr = ptr->next;
    }

    if (ptr == NULL) {
        /* we haven't found any suitable chunk */
        return NULL;
    }

    new_chunk = chunk_split(ptr, size);
    /* delete chunk from free list */
    ptr->next = new_chunk->next;
    new_chunk->next = NULL;
    new_chunk->flags = CHUNK_USED;
    return (void*)(new_chunk->data);
}

void kfree(void* p)
{
    struct chunk* ptr;
    struct chunk* chunk = (struct chunk*)(p - sizeof(struct chunk));

    // TODO: disable all irqs and suspend scheduler here

    struct chunk* left = chunk_left(chunk);
    struct chunk* right = chunk_right(chunk);

    if (left->flags & CHUNK_FREE) {
        /* our chunk and chunk to the left are free, merge them */
        chunk_merge(left, chunk);
        /*
         * Because of merging with left chunk, don't need to insert our chunk
         * in free list: it's already correctly inserted as part of left chunk.
         * So, all we need to do is merge with right chunk, if possible.
         */
        if (right->flags & CHUNK_FREE) {
            left->next = right->next;
            chunk_merge(left, right);
        }
    } else if (right->flags & CHUNK_FREE) {
        /* right chunk isn't used, find place in free list for freed chunk */
        ptr = head_chunk;
        while (ptr && ptr->next != right) {
            ptr = ptr->next;
        }

        if (ptr == NULL) {
            /* oops */
            return;
        }
        /* insert new chunk, delete right chunk and merge */
        ptr->next = chunk;
        chunk->next = right->next;
        chunk_merge(chunk, right);
        chunk->flags = CHUNK_FREE;
    } else {
        /* both left and right chunks are used, just insert chunk */
        ptr = head_chunk;
        while (ptr->next != NULL && ptr->next < chunk) {
            ptr = ptr->next;
        }

        chunk->next = ptr->next;
        ptr->next = chunk;
        chunk->flags = CHUNK_FREE;
    }
}
