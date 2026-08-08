#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstdint>

int main()
{
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    ASSERT_NULL(mem::malloc(SIZE_MAX));
    ASSERT_NULL(mem::malloc(SIZE_MAX - 1));
    ASSERT_NULL(mem::malloc(SIZE_MAX - 8));
    ASSERT_NULL(mem::malloc(SIZE_MAX - 16));

    ASSERT_NULL(mem::malloc(SIZE_MAX / 2));

    ASSERT_NULL(mem::malloc(static_cast<size_t>(1) << 63));

    size_t huge_size = static_cast<size_t>(1) << 40;
    void* huge_ptr = mem::malloc(huge_size);
    if (huge_ptr != nullptr)
    {
        mem::free(huge_ptr);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    char* ptr = static_cast<char*>(mem::malloc(32));
    ASSERT_NOT_NULL(ptr);
    ASSERT_NULL(mem::realloc(ptr, SIZE_MAX));

    for (size_t i = 0; i < 32; i++)
    {
        ptr[i] = static_cast<char>(i);
    }

    for (size_t i = 0; i < 32; i++)
    {
        ASSERT_TRUE(ptr[i] == static_cast<char>(i));
    }

    mem::free(ptr);

    ASSERT_TRUE(getTotalUsedMemory() == 0);
    TEST_PASS();
}
