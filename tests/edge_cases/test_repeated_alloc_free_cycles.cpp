#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int iterations = 20;

    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    for (int cycle = 0; cycle < iterations; cycle++)
    {
        std::cout << "TOTAL MEMORY MID 1: " << getTotalUsedMemory() << std::endl;
        ASSERT_TRUE(getTotalUsedMemory() == 0);

        char* block = static_cast<char*>(mem::malloc(4096));
        ASSERT_NOT_NULL(block);

        for (int i = 0; i < 4096; i++)
        {
            block[i] = static_cast<char>(cycle & 0xFF);
        }

        for (int i = 0; i < 4096; i++)
        {
            ASSERT_TRUE(block[i] == static_cast<char>(cycle & 0xFF));
        }

        std::cout << "TOTAL MEMORY MID 2: " << getTotalUsedMemory() << std::endl;
        ASSERT_TRUE(getTotalUsedMemory() >= 4096);

        mem::free(block);

        std::cout << "TOTAL MEMORY MID 3: " << getTotalUsedMemory() << std::endl;
        ASSERT_TRUE(getTotalUsedMemory() == 0);
    }

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
