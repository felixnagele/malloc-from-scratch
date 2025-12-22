#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    size_t initial_memory = getTotalUsedMemory();
    std::cout << "TOTAL MEMORY START: " << initial_memory << std::endl;

    char* block1 = static_cast<char*>(mem::malloc(100));
    ASSERT_NOT_NULL(block1);

    for (int i = 0; i < 100; i++)
    {
        block1[i] = static_cast<char>(i & 0x7F);
    }

    std::cout << "TOTAL MEMORY MID: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() >= 100);

    for (int i = 0; i < 100; i++)
    {
        ASSERT_TRUE(block1[i] == static_cast<char>(i & 0x7F));
    }

    mem::free(block1);

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == initial_memory);

    TEST_PASS();
}
