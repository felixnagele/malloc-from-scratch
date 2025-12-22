#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    const size_t size_a = 32;
    const size_t size_b = 48;
    const size_t size_c = 64;

    char* block_a = static_cast<char*>(mem::malloc(size_a));
    char* block_b = static_cast<char*>(mem::malloc(size_b));
    char* block_c = static_cast<char*>(mem::malloc(size_c));

    ASSERT_NOT_NULL(block_a);
    ASSERT_NOT_NULL(block_b);
    ASSERT_NOT_NULL(block_c);

    std::memset(block_a, 'A', size_a);
    std::memset(block_b, 'B', size_b);
    std::memset(block_c, 'C', size_c);

    for (size_t i = 0; i < size_a; i++)
    {
        ASSERT_TRUE(block_a[i] == 'A');
    }
    for (size_t i = 0; i < size_b; i++)
    {
        ASSERT_TRUE(block_b[i] == 'B');
    }
    for (size_t i = 0; i < size_c; i++)
    {
        ASSERT_TRUE(block_c[i] == 'C');
    }

    mem::free(block_b);

    size_t free_addr_1 = getFreeBlockInfo(0);
    size_t free_size_1 = getFreeBlockInfo(1);

    ASSERT_TRUE(free_addr_1 != 0);
    ASSERT_TRUE(reinterpret_cast<void*>(free_addr_1) == reinterpret_cast<void*>(block_b));
    ASSERT_TRUE(free_size_1 > 0);

    mem::free(block_a);
    mem::free(block_c);

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
