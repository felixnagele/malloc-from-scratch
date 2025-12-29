#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int num_pointers = 500;
    char* pointers[num_pointers];

    for (int i = 0; i < num_pointers; i++)
    {
        pointers[i] = static_cast<char*>(mem::malloc(32));
        ASSERT_NOT_NULL(pointers[i]);
        pointers[i][0] = static_cast<char>(i);
    }

    ASSERT_TRUE(getTotalUsedMemory() >= num_pointers * 32);

    for (int i = 0; i < num_pointers; i += 2)
    {
        mem::free(pointers[i]);
    }

    for (int i = 1; i < num_pointers; i += 2)
    {
        mem::free(pointers[i]);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* allocations[100];

    for (int i = 0; i < 100; i++)
    {
        allocations[i] = mem::malloc(i < 50 ? 1 : 8);
        ASSERT_NOT_NULL(allocations[i]);
        *static_cast<unsigned char*>(allocations[i]) = i & 0xFF;
    }

    for (int i = 0; i < 100; i++)
    {
        ASSERT_TRUE(*static_cast<unsigned char*>(allocations[i]) ==
                    static_cast<unsigned char>(i & 0xFF));
        mem::free(allocations[i]);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* blocks[200];

    for (int i = 0; i < 200; i++)
    {
        size_t size = (i % 2 == 0) ? 32 : 128;
        blocks[i] = mem::malloc(size);
        ASSERT_NOT_NULL(blocks[i]);
        std::memset(blocks[i], i & 0xFF, size);
    }

    for (int i = 1; i < 200; i += 2)
    {
        mem::free(blocks[i]);
        blocks[i] = nullptr;
    }

    for (int i = 0; i < 50; i++)
    {
        void* ptr = mem::malloc(64);
        if (ptr)
        {
            std::memset(ptr, 0xAA, 64);
            mem::free(ptr);
        }
    }

    for (int i = 0; i < 200; i++)
    {
        if (blocks[i])
            mem::free(blocks[i]);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);
    TEST_PASS();
}
