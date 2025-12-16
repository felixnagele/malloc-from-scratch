#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstddef>

namespace mem
{

void* malloc(size_t size);

void free(void* ptr);

void* calloc(size_t num, size_t size);

void* realloc(void* ptr, size_t new_size);

} // namespace mem

#endif // MEMORY_ALLOCATOR_H
