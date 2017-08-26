#include "kernel/alloc.h"
#include "kernel/types.h"
#include "kernel/printk.h"
#include "kernel/defs.h"
#include "kernel/memory.h"
#include "kernel/bug.h"

/*
 * This is implementation of malloc-like memory allocator.
 * Chunks stored with data in that format:
 * |...chunk_header...|...data...|...chunk_size (u32)...|
 * chunk_size stored for left chunk O(1) lookup.
 * Data size is always divisible by 4, so chunk_headers always are word-aligned.
 */

#define MM_RIGHT_BOUNDARY ((void*)(SRAM_END))

struct chunk {
    u32 size;
    u32 flags;
    struct chunk* next;

    u8 data[0];
};

struct chunk* head_chunk = NULL;

struct chunk* chunk_left(void* chunk)
{
    if (chunk == data_brk) {
        /* leftmost chunk */
        return NULL;
    }
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
    if (chunk >= MM_RIGHT_BOUNDARY) {
        return NULL;
    }
    return chunk;
}

void chunk_set_size(struct chunk* chunk, u32 size)
{
    chunk->size = size;
    *(u32*)(chunk->data + size) = size;
}

/*
 * Memory manager initialization routine.
 */
void mm_init()
{
    u32 size;

    head_chunk = data_brk;
    head_chunk->flags = CHUNK_FREE;
    /* heap is located in the middle of SRAM */
    size = (SRAM_END - (u32)data_brk) - sizeof(struct chunk) - sizeof(u32);
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
    /* place new_chunk right after of ptr */
    new_chunk = ((void*)ptr) + sizeof(struct chunk) + ptr->size + sizeof(u32);
    new_chunk->size = size;
    new_chunk->flags = ptr->flags;
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
    struct chunk* prev;
    struct chunk* alloc_chunk;
    u32 alloc_chunk_size;

    if (size & 3) {
        /* align up size to 4 bytes */
        size += 4 - (size & 3);
    }

    /* TODO: disable all irqs and suspend scheduler here */

    /* Ensure we have enough size to allocate chunk_header */
    alloc_chunk_size = size + sizeof(struct chunk) + sizeof(u32);
    prev = NULL;
    ptr = head_chunk;
    /* Find first chunk that can be split in two or it's size exatcly matches size */
    while (ptr && ptr->size < alloc_chunk_size && ptr->size < size) {
        prev = ptr;
        ptr = ptr->next;
    }

    if (ptr == NULL) {
        /* we haven't found any suitable chunk */
        return NULL;
    }

    if (ptr->size < alloc_chunk_size) {
        /* cannot split chunk, but there is enough space to allocate memory, use it */
        alloc_chunk = ptr;
        if (prev == NULL) {
            head_chunk = head_chunk->next;
        } else {
            prev->next = ptr->next;
        }
    } else {
        /* alloc_chunk is not inserted in free list */
        alloc_chunk = chunk_split(ptr, size);
    }
    alloc_chunk->flags = CHUNK_USED;
    alloc_chunk->next = NULL;
    return (void*)(alloc_chunk->data);
}

void kfree(void* p)
{
    struct chunk* ptr;
    struct chunk* chunk = (struct chunk*)(p - sizeof(struct chunk));

    // TODO: disable all irqs and suspend scheduler here

    struct chunk* left = chunk_left(chunk);
    struct chunk* right = chunk_right(chunk);

    if (left != NULL && left->flags & CHUNK_FREE) {
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
    } else if (right != NULL && right->flags & CHUNK_FREE) {
        /* right chunk isn't used, find place in free list for freed chunk */
        ptr = head_chunk;
        while (ptr && ptr->next != right) {
            ptr = ptr->next;
        }

        BUG_ON_NULL(ptr);

        /* insert new chunk, delete right chunk and merge */
        ptr->next = chunk;
        chunk->next = right->next;
        chunk_merge(chunk, right);
        chunk->flags = CHUNK_FREE;
    } else {
        /* both left and right chunks are used, just insert chunk */
        if (head_chunk == NULL) {
            head_chunk = chunk;
            chunk->next = NULL;
        } else {
            ptr = head_chunk;
            while (ptr->next != NULL && ptr->next < chunk) {
                ptr = ptr->next;
            }

            chunk->next = ptr->next;
            ptr->next = chunk;
        }
        chunk->flags = CHUNK_FREE;
    }
}
