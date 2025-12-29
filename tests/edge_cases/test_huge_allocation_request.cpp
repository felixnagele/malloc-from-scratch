#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstdint>

int main()
{
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    size_t huge_size = static_cast<size_t>(1) << 40;
    void* huge_ptr = mem::malloc(huge_size);

    ASSERT_NULL(huge_ptr);

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    size_t extreme_size = SIZE_MAX / 2;
    void* extreme_ptr = mem::malloc(extreme_size);

    if (extreme_ptr != nullptr)
    {
        mem::free(extreme_ptr);
    }
    else
    {
        ASSERT_NULL(extreme_ptr);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
