#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int num_iterations = 10;
    constexpr int blocks_per_iteration = 20;

    for (int iteration = 0; iteration < num_iterations; iteration++)
    {
        void* small_blocks[blocks_per_iteration];

        for (int i = 0; i < blocks_per_iteration; i++)
        {
            size_t size = 16 + (i * 32);
            small_blocks[i] = mem::malloc(size);
            ASSERT_NOT_NULL(small_blocks[i]);

            std::memset(small_blocks[i], i & 0xFF, size);
        }

        void* large_block1 = mem::malloc(4096);
        void* large_block2 = mem::calloc(100, sizeof(int));

        ASSERT_NOT_NULL(large_block1);
        ASSERT_NOT_NULL(large_block2);

        for (int i = 0; i < blocks_per_iteration; i += 3)
        {
            mem::free(small_blocks[i]);
            small_blocks[i] = nullptr;
        }

        void* resized_block = mem::realloc(large_block1, 8192);
        ASSERT_NOT_NULL(resized_block);

        for (int i = 0; i < blocks_per_iteration; i++)
        {
            if (small_blocks[i] != nullptr)
            {
                mem::free(small_blocks[i]);
            }
        }

        mem::free(resized_block);
        mem::free(large_block2);
    }

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
