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
    size_t magic_;
    size_t size_;
    bool allocated_;
    MemoryBlock* next_;
};

extern MemoryBlock* block_list_head;
extern MemoryBlock* block_list_tail;
extern void* heap_start;
extern void* highest_break;
extern size_t total_memory_allocated;
extern pthread_mutex_t allocator_mutex;

void* malloc_no_lock(size_t size);
void free_no_lock(void* ptr);
void* calloc_no_lock(size_t num, size_t size);
void* realloc_no_lock(void* ptr, size_t new_size);

constexpr size_t BLOCK_METADATA_SIZE = sizeof(MemoryBlock);
constexpr size_t CHUNK_SIZE = 65536;
constexpr size_t MAGIC_NUMBER = 0xDEADBEEF;
constexpr size_t CANARY_VALUE = 0xC0FFEE00;
constexpr size_t CANARY_SIZE = sizeof(size_t);

// malloc
void* increaseHeap(size_t size);
MemoryBlock* findLargeEnoughFreeMemoryBlock(MemoryBlock** block_list_head, size_t size);
void* splitFreeMemoryBlockIfPossible(MemoryBlock* new_block, size_t size);
void* getMemoryBlockSplitAddress(MemoryBlock* new_block, size_t size);
void insertMemoryBlockAtEnd(MemoryBlock** block_list_head, MemoryBlock* new_block);
void setCanary(MemoryBlock* block);

// free
bool decreaseHeap(MemoryBlock* block_heap_end);
void mergeFreeMemoryBlocks();
void getLastMemoryBlock(MemoryBlock** block_list_tail, MemoryBlock** block_previous_from_last);
bool isBlockCorrupted(MemoryBlock* block);

// helpers
void* getPayloadAddress(MemoryBlock* block);
MemoryBlock* getMetadata(void* payload_address);
MemoryBlock* getMemoryBlockFromAddress(void* address);
bool isPointerInHeap(void* ptr);
void* getErrorCodeInVoidPtr(size_t error_code);
bool isValidBlock(MemoryBlock* block);
bool checkCanary(MemoryBlock* block);

// Test helper functions to inspect allocator state
size_t getTotalUsedMemory();
size_t getFreeBlockInfo(int type);

} // namespace internal
} // namespace mem

#endif // MEMORY_INTERNAL_H
