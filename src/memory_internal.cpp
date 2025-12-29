#include "malloc_from_scratch/memory_internal.h"

namespace mem
{
namespace internal
{

MemoryBlock* block_list_head = nullptr;
MemoryBlock* block_list_tail = nullptr;
void* heap_start = nullptr;
size_t total_memory_allocated = 0;
pthread_mutex_t allocator_mutex = PTHREAD_MUTEX_INITIALIZER;

size_t getTotalUsedMemory() { return total_memory_allocated; }
size_t getFreeBlockInfo(int type)
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
