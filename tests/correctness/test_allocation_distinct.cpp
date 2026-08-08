#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

#include <cstdint>

int main()
{
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    constexpr int num_blocks = 64;
    struct Range
    {
        uintptr_t start;
        uintptr_t end;
    };
    Range ranges[num_blocks];

    for (int i = 0; i < num_blocks; i++)
    {
        size_t size = 1 + (i * 3);
        void* ptr = mem::malloc(size);
        ASSERT_NOT_NULL(ptr);
        ranges[i].start = reinterpret_cast<uintptr_t>(ptr);
        ranges[i].end = ranges[i].start + size;
    }

    for (int i = 0; i < num_blocks; i++)
    {
        for (int j = i + 1; j < num_blocks; j++)
        {
            ASSERT_TRUE(ranges[i].end <= ranges[j].start || ranges[j].end <= ranges[i].start);
        }
    }

    for (int i = 0; i < num_blocks; i++)
    {
        mem::free(reinterpret_cast<void*>(ranges[i].start));
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
