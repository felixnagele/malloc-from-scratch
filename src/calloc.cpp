#include "malloc_from_scratch/memory_allocator.h"
#include "malloc_from_scratch/memory_internal.h"

#include <cstring>

namespace mem
{

void* calloc(size_t num, size_t size)
{
    pthread_mutex_lock(&internal::allocator_mutex);
    void* ptr = internal::calloc_no_lock(num, size);
    pthread_mutex_unlock(&internal::allocator_mutex);
    return ptr;
}

namespace internal
{

void* calloc_no_lock(size_t num, size_t size)
{
    if (num == 0 || size == 0)
    {
        return nullptr;
    }

    size_t total_size = num * size;
    size_t overflow_check = total_size / num;
    if (overflow_check != size)
    {
        return nullptr;
    }

    void* memory_allocation = malloc_no_lock(total_size);
    if (!memory_allocation)
    {
        return nullptr;
    }

    memset(memory_allocation, 0, total_size);

    return memory_allocation;
}

} // namespace internal
} // namespace mem
