#include "memory_allocator.h"

namespace mem
{

void* realloc(void* ptr, size_t new_size)
{
    (void)ptr;
    (void)new_size;
    return nullptr;
}

} // namespace mem
