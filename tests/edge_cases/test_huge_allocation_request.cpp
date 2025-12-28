#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstdint>

int main()
{
    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    size_t huge_size = static_cast<size_t>(1) << 40; // 1 TB
    void* huge_ptr = mem::malloc(huge_size);

    ASSERT_NULL(huge_ptr);

    std::cout << "TOTAL MEMORY MID: " << getTotalUsedMemory() << std::endl;
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

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
