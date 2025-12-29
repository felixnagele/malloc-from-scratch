#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr size_t large_size_1 = 100 * 1024; // 100 KB
    constexpr size_t large_size_2 = 150 * 1024; // 150 KB

    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    char* block_a = static_cast<char*>(mem::malloc(large_size_1));
    ASSERT_NOT_NULL(block_a);

    for (size_t i = 0; i < large_size_1; i++)
    {
        block_a[i] = static_cast<char>(i & 0xFF);
    }

    char* block_b = static_cast<char*>(mem::malloc(large_size_2));
    ASSERT_NOT_NULL(block_b);

    for (size_t i = 0; i < large_size_2; i++)
    {
        block_b[i] = static_cast<char>(255 - (i & 0xFF));
    }

    size_t memory_used = getTotalUsedMemory();
    std::cout << "TOTAL MEMORY MID 1: " << memory_used << std::endl;
    ASSERT_TRUE(memory_used >= large_size_1 + large_size_2);

    for (size_t i = 0; i < large_size_1; i++)
    {
        ASSERT_TRUE(block_a[i] == static_cast<char>(i & 0xFF));
    }

    for (size_t i = 0; i < large_size_2; i++)
    {
        ASSERT_TRUE(block_b[i] == static_cast<char>(255 - (i & 0xFF)));
    }

    mem::free(block_a);
    mem::free(block_b);

    std::cout << "TOTAL MEMORY MID 2: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    char* block_x = static_cast<char*>(mem::malloc(1024));
    char* block_y = static_cast<char*>(mem::malloc(2048));
    ASSERT_NOT_NULL(block_x);
    ASSERT_NOT_NULL(block_y);

    std::memset(block_x, 0xAA, 1024);
    std::memset(block_y, 0xBB, 2048);

    for (int i = 0; i < 1024; i++)
    {
        ASSERT_TRUE(block_x[i] == static_cast<char>(0xAA));
    }
    for (int i = 0; i < 2048; i++)
    {
        ASSERT_TRUE(block_y[i] == static_cast<char>(0xBB));
    }

    mem::free(block_x);
    mem::free(block_y);

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
