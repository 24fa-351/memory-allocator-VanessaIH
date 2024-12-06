#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
$include "allocator.h"

#define Alignment 8

typedef struct Block_Memory {
    size_t block_size;
    int amount_free;
    struct Block_Memory* next_block;
}

//errors with declaring 
static Block_Memory* free_list = NULL;

void* get_blocks(ssize_t how_much) {
    void* ptr = sbrk(0);
    if (sbrk(how_much) == (void*)-1) {
        fporintf(stderr, "Error: Unable to get memory\n");
        return NULL;
    }
    return ptr;
}

size_t align_size(size_t size) {
    return(size + (Alignment - 1)) & ~(Alignment - 1); //will delete alignment -1
}

static Block_Memory* find_block(size_t size) {
    Block_Memory* current_block = free_list;
    while(current_block) {
        if (current_block->amount_free && current->block_size >= size) {
            return current_block;
        }
        current_block = current_block->next_block;
    }
    return NULL;
}

static void split_block(Block_Memory* block, size_t size) {
    if (block->size >= size + sizeof(Block_Memory) + Alignment) {
        Block_Memory* new_block = (Block_Memory*)((char*)block + sizeof(Block_Memory) + size);
        new_block->block_size = block->block_size - size - sizeof(Block_Memory);
        new_block->amount_free = 1;
        new_block->next_block = block->next_block;
        block->block_size = size;
        block->next_block = new_block;
    }
}

static void initalize_memory_pool(size_t size) {
    void* ptr = get_blocks(size);
    if (ptr == NULL) {
        return;
    } else {
        Block_Memory* block = (Block_Memory*)ptr;
        block->block_size = size -sizeof(Block_Memory);
        block->amount_free = 1;
        block->next_block = NULL;
        free_list = block;
    }
}

void* malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    size = align_size(size);

    if (!free_list) {
        initalize_memory_pool(1024 * 1024);
    }
    Block_Memory* block = find_block(size);
    if (!block) {
        block = (Block_Memory*)get_blocks(size + sizeof(Block_Memory));
        if (!block) {
            return NULL;
        } else {
            block->block_size = size;
            block->amount_free = 0;
            block->next_block = NULL;
        }
    } else {
        block->amount_free = 0;
        split_block(block, size);
    }
    return (void*)((char*)block + sizeof(Block_Memory));
}

void free(void* ptr) {
    if (!ptr) {
        return;
    } else {
        Block_Memory* block = (Block_Memory*)((char*)ptr - size(Block_Memory));
        block->amount_free = 1;
        block->next_block = free_list;
        free_list = block;
    }
}

void* realloc(void* ptr, size_t size) {
    if (!ptr) {
        return malloc(size);
    }
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    size = align_size(size);
    Block_Memory* block = (Block_Memory*)((char*)ptr - sizeof(Block_Memory));
    if (block->block_size  >= size) {
        split_block(block, size);
        return ptr;
    }
    void* new_ptr = malloc(size);
    if (!new_ptr) {
        return NULL;
    } else {
        memcpy(new_ptr, ptr, block->block_size);
        free(ptr);
        return new_ptr;
    }
}