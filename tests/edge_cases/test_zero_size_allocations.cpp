#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    void* ptr1 = mem::malloc(0);
    ASSERT_NULL(ptr1);

    void* ptr2 = mem::calloc(0, 16);
    ASSERT_NULL(ptr2);

    void* ptr3 = mem::calloc(16, 0);
    ASSERT_NULL(ptr3);

    void* ptr4 = mem::calloc(0, 0);
    ASSERT_NULL(ptr4);

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
