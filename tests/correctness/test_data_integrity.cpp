#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    constexpr int num_blocks = 50;
    void* blocks[num_blocks];

    for (int i = 0; i < num_blocks; i++)
    {
        size_t size = 100 + (i * 10);
        blocks[i] = mem::malloc(size);
        ASSERT_NOT_NULL(blocks[i]);
        std::memset(blocks[i], i, size);
    }

    for (int i = 0; i < num_blocks; i += 3)
    {
        mem::free(blocks[i]);
        blocks[i] = nullptr;
    }

    for (int i = 0; i < num_blocks; i++)
    {
        if (blocks[i] != nullptr)
        {
            size_t size = 100 + (i * 10);
            unsigned char* data = static_cast<unsigned char*>(blocks[i]);
            ASSERT_TRUE(data[0] == static_cast<unsigned char>(i));
            ASSERT_TRUE(data[size - 1] == static_cast<unsigned char>(i));
        }
    }

    for (int i = 0; i < num_blocks; i++)
    {
        if (blocks[i])
            mem::free(blocks[i]);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    char* ptrs[50];

    for (int i = 0; i < 50; i++)
    {
        size_t block_size = 16 * (i + 1);
        ptrs[i] = static_cast<char*>(mem::malloc(block_size));
        ASSERT_NOT_NULL(ptrs[i]);
        ptrs[i][0] = static_cast<char>(i);
    }

    for (int i = 0; i < 50; i += 2)
    {
        mem::free(ptrs[i]);
        ptrs[i] = nullptr;
    }

    for (int i = 1; i < 50; i += 2)
    {
        ASSERT_TRUE(ptrs[i][0] == static_cast<char>(i));
        mem::free(ptrs[i]);
    }

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

    mem::free(block_b);

    char* block_d = static_cast<char*>(mem::malloc(100));
    ASSERT_NOT_NULL(block_d);
    ASSERT_TRUE(block_d == block_b);

    std::memset(block_d, 'D', 100);
    for (int i = 0; i < 100; i++)
    {
        ASSERT_TRUE(block_d[i] == 'D');
    }

    for (size_t i = 0; i < 64; i++)
    {
        ASSERT_TRUE(block_a[i] == 'A');
    }
    for (size_t i = 0; i < 256; i++)
    {
        ASSERT_TRUE(block_c[i] == 'C');
    }

    mem::free(block_a);
    mem::free(block_c);
    mem::free(block_d);

    ASSERT_TRUE(getTotalUsedMemory() == 0);
    TEST_PASS();
}
