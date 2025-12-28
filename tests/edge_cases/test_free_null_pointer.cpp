#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    mem::free(nullptr);
    mem::free(nullptr);
    mem::free(nullptr);

    std::cout << "TOTAL MEMORY MID: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* ptr = mem::malloc(64);
    ASSERT_NOT_NULL(ptr);
    mem::free(ptr);

    mem::free(nullptr);

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
