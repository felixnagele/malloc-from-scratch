#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int num_pointers = 500;
    constexpr int block_size = 32;
    char* pointers[num_pointers];

    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    for (int i = 0; i < num_pointers; i++)
    {
        pointers[i] = static_cast<char*>(mem::malloc(block_size));
        ASSERT_NOT_NULL(pointers[i]);

        for (int j = 0; j < block_size; j++)
        {
            pointers[i][j] = static_cast<char>(i + j);
        }
    }

    std::cout << "TOTAL MEMORY MID 1: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() >= num_pointers * block_size);

    for (int i = 0; i < num_pointers; i++)
    {
        for (int j = 0; j < block_size; j++)
        {
            ASSERT_TRUE(pointers[i][j] == static_cast<char>(i + j));
        }
    }

    for (int i = 0; i < num_pointers; i += 2)
    {
        mem::free(pointers[i]);
    }

    std::cout << "TOTAL MEMORY MID 2: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() > 0);

    for (int i = 1; i < num_pointers; i += 2)
    {
        mem::free(pointers[i]);
    }

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
