#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

#include <cstring>

// The code that runs inside the forked child
void child_work()
{
    ASSERT_TRUE(getTotalUsedMemory() == 64);

    char* small = static_cast<char*>(mem::malloc(128));
    ASSERT_NOT_NULL(small);
    std::memset(small, 'C', 128);
    check_bytes(small, 128, 'C');
    mem::free(small);

    int* zeros = static_cast<int*>(mem::calloc(10, sizeof(int)));
    ASSERT_NOT_NULL(zeros);
    for (int i = 0; i < 10; i++)
    {
        ASSERT_TRUE(zeros[i] == 0);
    }
    mem::free(zeros);

    char* grown = static_cast<char*>(mem::malloc(64));
    ASSERT_NOT_NULL(grown);
    std::memset(grown, 'G', 64);

    grown = static_cast<char*>(mem::realloc(grown, 128));
    ASSERT_NOT_NULL(grown);
    check_bytes(grown, 64, 'G');

    grown = static_cast<char*>(mem::realloc(grown, 32));
    ASSERT_NOT_NULL(grown);
    check_bytes(grown, 32, 'G');
    mem::free(grown);

    constexpr size_t large_size = 8 * 1024;
    char* large = static_cast<char*>(mem::malloc(large_size));
    ASSERT_NOT_NULL(large);
    std::memset(large, 'L', large_size);
    check_bytes(large, large_size, 'L');
    mem::free(large);

    ASSERT_TRUE(getTotalUsedMemory() == 64);
}

int main()
{
    char* parent_block = static_cast<char*>(mem::malloc(64));
    ASSERT_NOT_NULL(parent_block);
    std::memset(parent_block, 'P', 64);
    ASSERT_TRUE(getTotalUsedMemory() == 64);

    run_in_child(0, child_work);

    check_bytes(parent_block, 64, 'P');
    ASSERT_TRUE(getTotalUsedMemory() == 64);

    mem::free(parent_block);
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
