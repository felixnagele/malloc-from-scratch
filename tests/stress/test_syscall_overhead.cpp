#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    // Limitations!
    // With locking, 10k allocations/free cycles can be done in under 1 second.
    // Without locking, 100k allocations/free cycles can be done in under 1 second.
    constexpr int num_allocations = 10000;

    int* buffers[num_allocations];

    for (int i = 0; i < num_allocations; i++)
    {
        buffers[i] = static_cast<int*>(mem::malloc(sizeof(int)));
        ASSERT_NOT_NULL(buffers[i]);
        *buffers[i] = i;
    }

    for (int i = 0; i < num_allocations; i += 1000)
    {
        ASSERT_TRUE(*buffers[i] == i);
    }

    for (int i = 0; i < num_allocations; i++)
    {
        mem::free(buffers[i]);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
