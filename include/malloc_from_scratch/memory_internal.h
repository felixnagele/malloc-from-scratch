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

// free
void decreaseHeap(MemoryBlock* block_heap_end);
void mergeFreeMemoryBlocks();
void getLastMemoryBlock(MemoryBlock** block_list_tail, MemoryBlock** block_previous_from_last);

// helpers
void* getPayloadAddress(MemoryBlock* block);
MemoryBlock* getMetadata(void* payload_address);
MemoryBlock* getMemoryBlockFromAddress(void* address);
size_t getSizeOfAllocatedMemoryBlock(MemoryBlock* block);
bool isPointerInHeap(void* ptr);
void* getErrorCodeInVoidPtr(size_t error_code);

// Test helper functions to inspect allocator state
inline size_t getTotalUsedMemory() { return total_memory_allocated; }
inline size_t getFreeBlockInfo(int type)
{
    // type 0 = address of first free block
    // type 1 = size of first free block
    MemoryBlock* current = block_list_head;
    while (current != nullptr)
    {
        if (!current->allocated_)
        {
            if (type == 0)
            {
                return reinterpret_cast<size_t>(getPayloadAddress(current));
            }
            else if (type == 1)
            {
                return current->size_;
            }
        }
        current = current->next_;
    }
    return 0;
}

} // namespace internal
} // namespace mem

#endif // MEMORY_INTERNAL_H
