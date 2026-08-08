#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr size_t large_size_1 = 100 * 1024;
    constexpr size_t large_size_2 = 150 * 1024;

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

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    char* block_c = static_cast<char*>(mem::malloc(large_size_1));
    ASSERT_NOT_NULL(block_c);

    for (size_t i = 0; i < large_size_1; i++)
    {
        block_c[i] = static_cast<char>(i & 0xFF);
    }

    for (size_t i = 0; i < large_size_1; i++)
    {
        ASSERT_TRUE(block_c[i] == static_cast<char>(i & 0xFF));
    }

    mem::free(block_c);

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
