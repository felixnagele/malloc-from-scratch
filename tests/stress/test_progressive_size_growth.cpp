#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>
#include <vector>

int main()
{
    constexpr int max_size = 128 * 1024;
    std::vector<void*> allocations;

    for (size_t size = 1; size <= max_size; size *= 2)
    {
        void* ptr = mem::malloc(size);
        ASSERT_NOT_NULL(ptr);

        std::memset(ptr, (size & 0xFF), size);
        allocations.push_back(ptr);
    }

    size_t size = 1;
    for (void* ptr : allocations)
    {
        unsigned char* data = static_cast<unsigned char*>(ptr);
        unsigned char expected = (size & 0xFF);

        ASSERT_TRUE(data[0] == expected);
        ASSERT_TRUE(data[size - 1] == expected);

        size *= 2;
    }

    for (void* ptr : allocations)
    {
        mem::free(ptr);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
