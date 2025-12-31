#include "malloc_from_scratch/memory_allocator.h"
#include "malloc_from_scratch/memory_internal.h"

#include <cstdlib>
#include <cstring>

namespace mem
{

void* realloc(void* ptr, size_t new_size)
{
    if (ptr == nullptr)
    {
        return malloc(new_size);
    }

    if (new_size == 0)
    {
        free(ptr);
        return nullptr;
    }

    pthread_mutex_lock(&internal::allocator_mutex);
    void* result = internal::realloc_no_lock(ptr, new_size);
    pthread_mutex_unlock(&internal::allocator_mutex);
    return result;
}

namespace internal
{

void* realloc_no_lock(void* ptr, size_t new_size)
{
    if (!isPointerInHeap(ptr))
    {
        return nullptr;
    }

    MemoryBlock* old_block = getMetadata(ptr);
    if (!isValidBlock(old_block))
    {
        return nullptr;
    }

    if (!checkCanary(old_block))
    {
        exit(-1);
    }

    size_t old_size = old_block->size_;

    if (new_size <= old_size)
    {
        return ptr;
    }

    void* new_ptr = malloc_no_lock(new_size);
    if (new_ptr == nullptr)
    {
        return nullptr;
    }

    size_t copy_size;
    if (old_size < new_size)
    {
        copy_size = old_size;
    }
    else
    {
        copy_size = new_size;
    }
    memcpy(new_ptr, ptr, copy_size);

    free_no_lock(ptr);

    return new_ptr;
}

} // namespace internal
} // namespace mem
