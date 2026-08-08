#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

#include <limits>

int main()
{
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    size_t max = std::numeric_limits<size_t>::max();

    ASSERT_NULL(mem::calloc(max / 2 + 1, 2));
    ASSERT_NULL(mem::calloc(max, 2));
    ASSERT_NULL(mem::calloc(1, max));
    ASSERT_NULL(mem::calloc(max, max));
    ASSERT_NULL(mem::calloc(2, max / 2 + 1));

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* ptr = mem::calloc(100, 4);
    ASSERT_NOT_NULL(ptr);

    unsigned char* data = static_cast<unsigned char*>(ptr);
    for (int i = 0; i < 400; i++)
    {
        ASSERT_TRUE(data[i] == 0);
    }
    mem::free(ptr);

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
