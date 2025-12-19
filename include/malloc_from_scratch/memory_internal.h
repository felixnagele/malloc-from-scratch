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
constexpr size_t BLOCK_SIZE = 16;
constexpr size_t CHUNK_SIZE = 65536;

// malloc
void* increaseHeap(size_t size);
MemoryBlock* findLargeEnoughFreeMemoryBlock(MemoryBlock** block_list_head, size_t size);
void* splitFreeMemoryBlockIfPossible(MemoryBlock* new_block, size_t size);
void* getMemoryBlockSplitAddress(MemoryBlock* new_block, size_t size);
void insertMemoryBlockAtEnd(MemoryBlock** block_list_head, MemoryBlock* new_block);

// helpers
void* getPayloadAddress(MemoryBlock* block);
MemoryBlock* getMetadata(void* payload_address);
MemoryBlock* getMemoryBlockFromAddress(void* address);
size_t getSizeOfAllocatedMemoryBlock(MemoryBlock* block);
void* getErrorCodeInVoidPtr(size_t error_code);

} // namespace internal
} // namespace mem

#endif // MEMORY_INTERNAL_H
