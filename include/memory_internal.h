#ifndef MEMORY_INTERNAL_H
#define MEMORY_INTERNAL_H

#include <cstddef>
#include <pthread.h>

namespace mem
{
namespace internal
{

struct MemoryBlock
{
    size_t size;
    bool allocated;
    MemoryBlock* next;
};

extern MemoryBlock* block_list_head;
extern void* heap_start;
extern pthread_mutex_t allocator_mutex;

constexpr size_t BLOCK_METADATA_SIZE = sizeof(MemoryBlock);
constexpr size_t MIN_BLOCK_SIZE = 16;

} // namespace internal
} // namespace mem

#endif // MEMORY_INTERNAL_H
