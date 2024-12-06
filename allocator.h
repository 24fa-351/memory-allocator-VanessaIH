#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

void* get_blocks(ssize_t how_much);
void* malloc(size_t size);
void free(void* ptr);
void* realloc(void* ptr, size_t size);

#endif