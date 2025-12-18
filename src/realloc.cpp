#include "malloc_from_scratch/memory_allocator.h"

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

    void* new_ptr = malloc(new_size);
    if (new_ptr == nullptr)
    {
        return nullptr;
    }

    free(ptr);

    return new_ptr;
}

} // namespace mem
