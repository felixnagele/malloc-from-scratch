#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    char* block_a = static_cast<char*>(mem::malloc(64));
    char* block_b = static_cast<char*>(mem::malloc(128));
    char* block_c = static_cast<char*>(mem::malloc(256));

    ASSERT_NOT_NULL(block_a);
    ASSERT_NOT_NULL(block_b);
    ASSERT_NOT_NULL(block_c);

    std::memset(block_a, 'A', 64);
    std::memset(block_b, 'B', 128);
    std::memset(block_c, 'C', 256);

    for (size_t i = 0; i < 64; i++)
    {
        ASSERT_TRUE(block_a[i] == 'A');
    }
    for (size_t i = 0; i < 128; i++)
    {
        ASSERT_TRUE(block_b[i] == 'B');
    }
    for (size_t i = 0; i < 256; i++)
    {
        ASSERT_TRUE(block_c[i] == 'C');
    }

    mem::free(block_b);

    size_t free_address = getFreeBlockInfo(0);
    size_t free_size = getFreeBlockInfo(1);

    ASSERT_TRUE(free_address != 0);
    ASSERT_TRUE(free_address == reinterpret_cast<size_t>(block_b));
    ASSERT_TRUE(free_size >= 128);

    char* block_d = static_cast<char*>(mem::malloc(100));
    ASSERT_NOT_NULL(block_d);

    ASSERT_TRUE(block_d == block_b);

    std::memset(block_d, 'D', 100);

    for (int i = 0; i < 100; i++)
    {
        ASSERT_TRUE(block_d[i] == 'D');
    }

    mem::free(block_a);
    mem::free(block_c);
    mem::free(block_d);

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
