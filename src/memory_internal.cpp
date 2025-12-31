#include "malloc_from_scratch/memory_internal.h"

namespace mem
{
namespace internal
{

MemoryBlock* block_list_head = nullptr;
MemoryBlock* block_list_tail = nullptr;
void* heap_start = nullptr;
void* highest_break = nullptr;
size_t total_memory_allocated = 0;
pthread_mutex_t allocator_mutex = PTHREAD_MUTEX_INITIALIZER;

size_t getTotalUsedMemory()
{
    pthread_mutex_lock(&allocator_mutex);
    size_t result = total_memory_allocated;
    pthread_mutex_unlock(&allocator_mutex);
    return result;
}

size_t getFreeBlockInfo(int type)
{
    pthread_mutex_lock(&allocator_mutex);

    MemoryBlock* current = block_list_head;
    while (current != nullptr)
    {
        if (!current->allocated_)
        {
            if (type == 0)
            {
                pthread_mutex_unlock(&allocator_mutex);
                return reinterpret_cast<size_t>(getPayloadAddress(current));
            }
            else if (type == 1)
            {
                pthread_mutex_unlock(&allocator_mutex);
                return current->size_;
            }
        }
        current = current->next_;
    }
    pthread_mutex_unlock(&allocator_mutex);
    return 0;
}

} // namespace internal
} // namespace mem
