#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int iterations = 1000;

    for (int i = 0; i < iterations; i++)
    {
        size_t size = ((i * 7) % 128) + 1;
        char* ptr = static_cast<char*>(mem::malloc(size));
        ASSERT_NOT_NULL(ptr);
        std::memset(ptr, i & 0xFF, size);
        mem::free(ptr);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    for (int cycle = 0; cycle < 100; cycle++)
    {
        void* ptrs[10];
        for (int i = 0; i < 10; i++)
        {
            size_t size = (i + 1) * 16;
            ptrs[i] = mem::malloc(size);
            ASSERT_NOT_NULL(ptrs[i]);
            std::memset(ptrs[i], i, size);
        }

        for (int i = 9; i >= 0; --i)
        {
            mem::free(ptrs[i]);
        }
    }

    for (int cycle = 0; cycle < 100; cycle++)
    {
        void* ptrs[10];
        for (int i = 0; i < 10; i++)
        {
            size_t size = (10 - i) * 16;
            ptrs[i] = mem::malloc(size);
            ASSERT_NOT_NULL(ptrs[i]);
            std::memset(ptrs[i], i, size);
        }

        for (int i = 0; i < 10; i++)
        {
            mem::free(ptrs[i]);
        }
    }

    constexpr int max_blocks = 50;
    char* pointers[max_blocks];

    for (int iteration = 0; iteration < 100; iteration++)
    {
        for (int i = 0; i < max_blocks; i++)
        {
            size_t size = (i + 1) * (iteration % 4 + 1);
            pointers[i] = static_cast<char*>(mem::malloc(size));
            ASSERT_NOT_NULL(pointers[i]);
            pointers[i][0] = static_cast<char>(i);
        }

        for (int i = 1; i < max_blocks; i += 3)
        {
            mem::free(pointers[i]);
            pointers[i] = nullptr;
        }

        for (int i = 0; i < max_blocks; i++)
        {
            if (pointers[i] != nullptr)
            {
                ASSERT_TRUE(pointers[i][0] == static_cast<char>(i));
                mem::free(pointers[i]);
            }
        }
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);
    TEST_PASS();
}
