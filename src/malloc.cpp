#include "malloc_from_scratch/memory_allocator.h"
#include "malloc_from_scratch/memory_internal.h"

#include <cstdlib>
#include <unistd.h>

namespace mem
{

void* malloc(size_t size)
{
    if (size == 0)
    {
        return nullptr;
    }

    if (internal::heap_start == nullptr)
    {
        internal::heap_start = sbrk(0);
    }

    internal::MemoryBlock* found_block =
        internal::findLargeEnoughFreeMemoryBlock(&internal::block_list_head, size);

    if (found_block != nullptr)
    {
        void* return_address = internal::splitFreeMemoryBlockIfPossible(found_block, size);
        return return_address;
    }

    bool is_frame_used = false;
    size_t total_size = 0;
    if (size < internal::CHUNK_SIZE)
    {
        total_size = internal::BLOCK_METADATA_SIZE + internal::CHUNK_SIZE;
        is_frame_used = true;
    }
    else
    {
        total_size = internal::BLOCK_METADATA_SIZE + size + internal::CANARY_SIZE;
        is_frame_used = false;
    }

    void* new_memory_allocation = internal::increaseHeap(total_size);
    if (new_memory_allocation == nullptr)
    {
        return nullptr;
    }

    internal::MemoryBlock* new_block = internal::getMemoryBlockFromAddress(new_memory_allocation);
    if (is_frame_used)
    {
        new_block->magic_ = internal::MAGIC_NUMBER;
        new_block->size_ = internal::CHUNK_SIZE;
        new_block->allocated_ = false;
        new_block->next_ = nullptr;
        internal::insertMemoryBlockAtEnd(&internal::block_list_head, new_block);
        void* return_address = internal::splitFreeMemoryBlockIfPossible(new_block, size);
        return return_address;
    }
    else
    {
        new_block->magic_ = internal::MAGIC_NUMBER;
        new_block->size_ = size;
        new_block->allocated_ = true;
        new_block->next_ = nullptr;
        internal::insertMemoryBlockAtEnd(&internal::block_list_head, new_block);
        internal::setCanary(new_block);
        internal::total_memory_allocated += size;
    }

    return internal::getPayloadAddress(new_block);
}

namespace internal
{

void* increaseHeap(size_t size)
{
    void* result = sbrk(static_cast<intptr_t>(size));
    if (result == getErrorCodeInVoidPtr(-1) || result == getErrorCodeInVoidPtr(0))
    {
        return nullptr;
    }

    return result;
}

MemoryBlock* findLargeEnoughFreeMemoryBlock(MemoryBlock** block_list_head, size_t size)
{
    MemoryBlock* current = *block_list_head;
    size_t size_needed = size + CANARY_SIZE;
    while (current != nullptr)
    {
        if (current->allocated_ == false && current->size_ >= size_needed)
        {
            return current;
        }
        current = current->next_;
    }

    return nullptr;
}

void* splitFreeMemoryBlockIfPossible(MemoryBlock* new_block, size_t size)
{
    if (new_block == nullptr)
    {
        return nullptr;
    }

    size_t size_with_canary = size + CANARY_SIZE;
    size_t remaining_size = new_block->size_ - size_with_canary;
    char one_byte_payload_size_requirement = 1;
    if (remaining_size >= BLOCK_METADATA_SIZE + one_byte_payload_size_requirement)
    {
        void* split_address = getMemoryBlockSplitAddress(new_block, size);
        MemoryBlock* new_temp_block = getMemoryBlockFromAddress(split_address);
        new_temp_block->magic_ = MAGIC_NUMBER;
        new_temp_block->size_ = remaining_size - BLOCK_METADATA_SIZE;
        new_temp_block->allocated_ = false;
        new_temp_block->next_ = new_block->next_;

        new_block->size_ = size;
        new_block->allocated_ = true;
        new_block->next_ = new_temp_block;
    }
    else
    {
        new_block->size_ = new_block->size_ - CANARY_SIZE;
        new_block->allocated_ = true;
    }

    setCanary(new_block);
    total_memory_allocated += new_block->size_;

    return getPayloadAddress(new_block);
}

void* getMemoryBlockSplitAddress(MemoryBlock* new_block, size_t size)
{
    return (reinterpret_cast<char*>(new_block) + BLOCK_METADATA_SIZE + size + CANARY_SIZE);
}

void insertMemoryBlockAtEnd(MemoryBlock** block_list_head, MemoryBlock* new_block)
{
    new_block->next_ = nullptr;

    if (*block_list_head == nullptr)
    {
        *block_list_head = new_block;
        block_list_tail = new_block;
    }
    else
    {
        block_list_tail->next_ = new_block;
        block_list_tail = new_block;
    }
}

void setCanary(MemoryBlock* block)
{
    if (!block || block->allocated_ == false)
    {
        return;
    }

    size_t* canary = reinterpret_cast<size_t*>(reinterpret_cast<char*>(block) +
                                               BLOCK_METADATA_SIZE + block->size_);
    *canary = CANARY_VALUE;
}

// helpers
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
    return reinterpret_cast<MemoryBlock*>(reinterpret_cast<char*>(payload_address) -
                                          BLOCK_METADATA_SIZE);
}

MemoryBlock* getMemoryBlockFromAddress(void* address)
{
    return reinterpret_cast<MemoryBlock*>(address);
}

bool isPointerInHeap(void* ptr)
{
    void* current_program_break = sbrk(0);
    return (ptr >= heap_start && ptr < current_program_break);
}

void* getErrorCodeInVoidPtr(size_t error_code) { return reinterpret_cast<void*>(error_code); }

bool isValidBlock(MemoryBlock* block)
{
    return block != nullptr && block->magic_ == MAGIC_NUMBER && block->allocated_ == true;
}

bool checkCanary(MemoryBlock* block)
{
    if (!block)
    {
        return false;
    }

    size_t* canary = reinterpret_cast<size_t*>(reinterpret_cast<char*>(block) +
                                               BLOCK_METADATA_SIZE + block->size_);

    return (*canary == CANARY_VALUE);
}

} // namespace internal
} // namespace mem
