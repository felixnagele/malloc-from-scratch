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

    if (!internal::isPointerInHeap(ptr))
    {
        return nullptr;
    }

    internal::MemoryBlock* old_block = internal::getMetadata(ptr);
    if (!internal::isValidBlock(old_block))
    {
        return nullptr;
    }

    if (!internal::checkCanary(old_block))
    {
        exit(-1);
    }

    size_t old_size = old_block->size_;

    void* new_ptr = malloc(new_size);
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

    free(ptr);

    return new_ptr;
}

} // namespace mem
