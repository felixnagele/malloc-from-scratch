#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int num_allocations = 500;

    for (int i = 0; i < num_allocations; i++)
    {
        size_t size = ((i * 7) % 128) + 1;
        char* ptr = static_cast<char*>(mem::malloc(size));
        ASSERT_NOT_NULL(ptr);

        std::memset(ptr, i & 0xFF, size);

        mem::free(ptr);
    }

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
