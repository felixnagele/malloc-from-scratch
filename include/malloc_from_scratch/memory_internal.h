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
    size_t size_;
    bool allocated_;
    MemoryBlock* next_;
};

inline MemoryBlock* block_list_head = nullptr;
inline void* heap_start = nullptr;
inline size_t total_memory_allocated = 0;
inline pthread_mutex_t allocator_mutex = PTHREAD_MUTEX_INITIALIZER;

constexpr size_t BLOCK_METADATA_SIZE = sizeof(MemoryBlock);
constexpr size_t MIN_BLOCK_SIZE = 16;

void insertMemoryBlockAtEnd(MemoryBlock** block_list_head, MemoryBlock* new_block);
void* getPayloadAddress(MemoryBlock* block);
MemoryBlock* getMetadata(void* payload_address);
void* increaseHeap(size_t size);
void* decreaseHeap(MemoryBlock* block_heap_end, size_t size);

} // namespace internal
} // namespace mem

#endif // MEMORY_INTERNAL_H
