#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    constexpr int num_allocations = 100000;

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

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
