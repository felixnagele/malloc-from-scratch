#include "malloc_from_scratch/memory_allocator.h"
#include "malloc_from_scratch/memory_internal.h"

#include <unistd.h>

namespace mem
{

void* malloc(size_t size)
{
    if (internal::heap_start == nullptr)
    {
        internal::heap_start = sbrk(0);
    }

    internal::total_memory_allocated += size;

    size_t total_size = sizeof(internal::MemoryBlock) + size;

    void* new_memory_allocation = internal::increaseHeap(total_size);
    if (new_memory_allocation == NULL)
    {
        return NULL;
    }

    internal::MemoryBlock* new_block =
        reinterpret_cast<internal::MemoryBlock*>(new_memory_allocation);
    new_block->size_ = size;
    new_block->allocated_ = 1;
    new_block->next_ = nullptr;

    internal::insertMemoryBlockAtEnd(&internal::block_list_head, new_block);

    return internal::getPayloadAddress(new_block);
}

namespace internal
{

void insertMemoryBlockAtEnd(MemoryBlock** block_list_head, MemoryBlock* new_block)
{
    if (*block_list_head == nullptr)
    {
        *block_list_head = new_block;
    }
    else
    {
        MemoryBlock* current = *block_list_head;
        while (current->next_ != nullptr)
        {
            current = current->next_;
        }
        current->next_ = new_block;
    }
}

void* getPayloadAddress(MemoryBlock* block)
{
    if (!block)
    {
        return nullptr;
    }

    return reinterpret_cast<void*>(reinterpret_cast<char*>(block) + BLOCK_METADATA_SIZE);
}

MemoryBlock* getMetadata(void* payload_address)
{
    if (!payload_address)
    {
        return nullptr;
    }

    return reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(payload_address) -
                                          BLOCK_METADATA_SIZE);
}

void* increaseHeap(size_t size)
{
    void* result = sbrk(static_cast<intptr_t>(size));
    if (result == reinterpret_cast<void*>(-1))
    {
        return nullptr;
    }

    return result;
}

void* decreaseHeap(MemoryBlock* block_heap_end, size_t size) { return nullptr; }
} // namespace internal
} // namespace mem
