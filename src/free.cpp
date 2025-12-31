#include "malloc_from_scratch/memory_allocator.h"
#include "malloc_from_scratch/memory_internal.h"

#include <cstdlib>
#include <unistd.h>

namespace mem
{

void free(void* ptr)
{
    pthread_mutex_lock(&internal::allocator_mutex);
    internal::free_no_lock(ptr);
    pthread_mutex_unlock(&internal::allocator_mutex);
}

namespace internal
{

void free_no_lock(void* ptr)
{
    if (ptr == nullptr)
    {
        return;
    }

    if (!isPointerInHeap(ptr))
    {
        return;
    }

    MemoryBlock* block_to_free = getMetadata(ptr);
    if (!isValidBlock(block_to_free))
    {
        if (isBlockCorrupted(block_to_free))
        {
            exit(-1);
        }
        return;
    }

    if (!checkCanary(block_to_free))
    {
        exit(-1);
    }

    total_memory_allocated -= block_to_free->size_;
    block_to_free->allocated_ = false;

    mergeFreeMemoryBlocks();

    MemoryBlock* last_block = nullptr;
    MemoryBlock* second_to_last_block = nullptr;

    getLastMemoryBlock(&last_block, &second_to_last_block);
    if (last_block != nullptr && last_block->allocated_ == false)
    {
        bool success = decreaseHeap(last_block);
        if (success)
        {
            if (second_to_last_block != nullptr)
            {
                second_to_last_block->next_ = nullptr;
                block_list_tail = second_to_last_block;
            }
            else
            {
                block_list_head = nullptr;
                block_list_tail = nullptr;
            }
        }
    }
}

bool decreaseHeap(MemoryBlock* block_heap_end)
{
    if (block_heap_end == nullptr)
    {
        return false;
    }
    if (block_heap_end->next_ != nullptr)
    {
        return false;
    }

    void* current_break = sbrk(0);
    void* block_start = reinterpret_cast<void*>(block_heap_end);

    if (current_break != highest_break)
    {
        return false;
    }

    intptr_t release_size =
        reinterpret_cast<char*>(current_break) - reinterpret_cast<char*>(block_start);

    if (release_size <= 0)
    {
        return false;
    }

    void* program_break = sbrk(-release_size);

    if (program_break == getErrorCodeInVoidPtr(-1) || program_break == getErrorCodeInVoidPtr(0))
    {
        exit(-1);
    }

    highest_break = reinterpret_cast<char*>(current_break) - release_size;

    return true;
}

void mergeFreeMemoryBlocks()
{
    MemoryBlock* current = block_list_head;
    MemoryBlock* last = nullptr;

    while (current != nullptr && current->next_ != nullptr)
    {
        if (current->allocated_ == false && current->next_->allocated_ == false)
        {
            current->size_ += BLOCK_METADATA_SIZE + CANARY_SIZE + current->next_->size_;
            current->next_ = current->next_->next_;
        }
        else
        {
            last = current;
            current = current->next_;
        }
    }

    if (current != nullptr)
    {
        block_list_tail = current;
    }
    else if (last != nullptr)
    {
        block_list_tail = last;
    }
    else
    {
        block_list_tail = block_list_head;
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
