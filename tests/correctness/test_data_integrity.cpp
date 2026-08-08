#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

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
        fill_bytes(blocks[i], size, static_cast<unsigned char>(i));
    }

    for (int i = 0; i < num_blocks; i += 2)
    {
        mem::free(blocks[i]);
        blocks[i] = nullptr;
    }

    for (int i = 0; i < num_blocks; i++)
    {
        if (blocks[i] != nullptr)
        {
            size_t size = 100 + (i * 10);
            check_bytes(blocks[i], size, static_cast<unsigned char>(i));
        }
    }

    for (int i = 0; i < num_blocks; i++)
    {
        if (blocks[i])
            mem::free(blocks[i]);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    char* block_a = static_cast<char*>(mem::malloc(64));
    char* block_b = static_cast<char*>(mem::malloc(128));
    char* block_c = static_cast<char*>(mem::malloc(256));

    ASSERT_NOT_NULL(block_a);
    ASSERT_NOT_NULL(block_b);
    ASSERT_NOT_NULL(block_c);

    fill_bytes(block_a, 64, 'A');
    fill_bytes(block_b, 128, 'B');
    fill_bytes(block_c, 256, 'C');

    mem::free(block_b);

    char* block_d = static_cast<char*>(mem::malloc(100));
    ASSERT_NOT_NULL(block_d);
    ASSERT_TRUE(block_d == block_b);

    fill_bytes(block_d, 100, 'D');
    check_bytes(block_d, 100, 'D');
    check_bytes(block_a, 64, 'A');
    check_bytes(block_c, 256, 'C');

    mem::free(block_a);
    mem::free(block_c);
    mem::free(block_d);

    ASSERT_TRUE(getTotalUsedMemory() == 0);
    TEST_PASS();
}
