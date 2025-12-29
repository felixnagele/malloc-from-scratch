#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstddef>
#include <limits>

int main()
{
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    size_t huge = std::numeric_limits<size_t>::max() / 2 + 1;
    void* ptr = mem::calloc(huge, 2);
    ASSERT_NULL(ptr);

    void* ptr2 = mem::calloc(std::numeric_limits<size_t>::max(), 2);
    ASSERT_NULL(ptr2);

    void* ptr3 = mem::calloc(10, sizeof(int));
    ASSERT_NOT_NULL(ptr3);

    int* arr = static_cast<int*>(ptr3);
    for (int i = 0; i < 10; i++)
    {
        ASSERT_TRUE(arr[i] == 0);
    }

    mem::free(ptr3);

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
