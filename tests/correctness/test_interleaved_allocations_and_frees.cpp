#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int num_blocks = 50;
    char* blocks[num_blocks];

    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    for (int i = 0; i < num_blocks; i++)
    {
        size_t block_size = 16 * (i + 1);
        blocks[i] = static_cast<char*>(mem::malloc(block_size));
        ASSERT_NOT_NULL(blocks[i]);

        for (size_t j = 0; j < block_size; j++)
        {
            blocks[i][j] = static_cast<char>((i + j) & 0x7F);
        }
    }

    size_t memory_after_alloc = getTotalUsedMemory();
    std::cout << "TOTAL MEMORY MID 1: " << memory_after_alloc << std::endl;
    ASSERT_TRUE(memory_after_alloc > 0);

    for (int i = 0; i < num_blocks; i += 2)
    {
        mem::free(blocks[i]);
        blocks[i] = nullptr;
    }

    size_t memory_after_partial_free = getTotalUsedMemory();
    std::cout << "TOTAL MEMORY MID 2: " << memory_after_partial_free << std::endl;
    ASSERT_TRUE(memory_after_partial_free > 0);
    ASSERT_TRUE(memory_after_partial_free < memory_after_alloc);

    for (int i = 1; i < num_blocks; i += 2)
    {
        size_t block_size = 16 * (i + 1);
        for (size_t j = 0; j < block_size; j++)
        {
            ASSERT_TRUE(blocks[i][j] == static_cast<char>((i + j) & 0x7F));
        }
    }

    for (int i = 1; i < num_blocks; i += 2)
    {
        mem::free(blocks[i]);
        blocks[i] = nullptr;
    }

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
