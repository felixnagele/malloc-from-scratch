#include "malloc_from_scratch/memory_allocator.h"
#include "malloc_from_scratch/memory_internal.h"

#include <cstdlib>
#include <unistd.h>

namespace mem
{

void free(void* ptr)
{
    if (ptr == nullptr)
    {
        return;
    }

    if (!internal::isPointerInHeap(ptr))
    {
        return;
    }

    internal::MemoryBlock* block_to_free = internal::getMetadata(ptr);
    if (!internal::isValidBlock(block_to_free))
    {
        if (internal::isBlockCorrupted(block_to_free))
        {
            exit(-1);
        }
        return;
    }

    if (!internal::checkCanary(block_to_free))
    {
        exit(-1);
    }

    internal::total_memory_allocated -= block_to_free->size_;
    block_to_free->allocated_ = false;

    internal::mergeFreeMemoryBlocks();

    internal::MemoryBlock* block_list_tail = nullptr;
    internal::MemoryBlock* block_previous_from_last = nullptr;

    internal::getLastMemoryBlock(&block_list_tail, &block_previous_from_last);
    if (block_list_tail != nullptr && block_list_tail->allocated_ == false)
    {
        if (block_previous_from_last != nullptr)
        {
            block_previous_from_last->next_ = nullptr;
        }
        else
        {
            internal::block_list_head = nullptr;
        }
        internal::decreaseHeap(block_list_tail);
    }
}

namespace internal
{

void decreaseHeap(MemoryBlock* block_heap_end)
{
    if (block_heap_end == nullptr)
    {
        return;
    }
    if (block_heap_end->next_ != nullptr)
    {
        return;
    }

    void* current_break = sbrk(0);
    void* block_start = reinterpret_cast<void*>(block_heap_end);
    intptr_t release_size =
        reinterpret_cast<char*>(current_break) - reinterpret_cast<char*>(block_start);

    if (release_size <= 0)
    {
        return;
    }

    void* program_break = sbrk(-release_size);

    if (program_break == getErrorCodeInVoidPtr(-1) || program_break == getErrorCodeInVoidPtr(0))
    {
        exit(-1);
    }
}

void mergeFreeMemoryBlocks()
{
    MemoryBlock* current = block_list_head;
    while (current != nullptr && current->next_ != nullptr)
    {
        if (current->allocated_ == false && current->next_->allocated_ == false)
        {
            current->size_ += BLOCK_METADATA_SIZE + CANARY_SIZE + current->next_->size_;
            current->next_ = current->next_->next_;
        }
        else
        {
            current = current->next_;
        }
    }
}

void getLastMemoryBlock(MemoryBlock** block_list_tail, MemoryBlock** block_previous_from_last)
{
    MemoryBlock* current = block_list_head;
    MemoryBlock* previous = nullptr;

    while (current != nullptr && current->next_ != nullptr)
    {
        previous = current;
        current = current->next_;
    }

    *block_list_tail = current;
    *block_previous_from_last = previous;
}

bool isBlockCorrupted(MemoryBlock* block) { return (block && block->magic_ != MAGIC_NUMBER); }

} // namespace internal
} // namespace mem
