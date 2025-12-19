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

    void* current_program_break = sbrk(0);

    if (ptr < internal::heap_start || ptr >= current_program_break)
    {
        return;
    }

    internal::MemoryBlock* block_to_free = internal::getMetadata(ptr);
    if (block_to_free == nullptr || block_to_free->allocated_ == false)
    {
        return;
    }
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

    size_t release_size = block_heap_end->size_ + BLOCK_METADATA_SIZE;
    void* program_break = sbrk(-static_cast<intptr_t>(release_size));

    if (program_break == getErrorCodeInVoidPtr(-1) || program_break == getErrorCodeInVoidPtr(0))
    {
        exit(-1);
    }

    total_memory_allocated -= release_size;
}

void mergeFreeMemoryBlocks()
{
    MemoryBlock* current = block_list_head;
    while (current != nullptr && current->next_ != nullptr)
    {
        if (current->allocated_ == false && current->next_->allocated_ == false)
        {
            current->size_ += BLOCK_METADATA_SIZE + current->next_->size_;
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

} // namespace internal
} // namespace mem
