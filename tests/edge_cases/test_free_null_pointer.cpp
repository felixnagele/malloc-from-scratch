#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    mem::free(nullptr);
    mem::free(nullptr);
    mem::free(nullptr);

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* ptr = mem::malloc(64);
    ASSERT_NOT_NULL(ptr);
    mem::free(ptr);

    mem::free(nullptr);

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
