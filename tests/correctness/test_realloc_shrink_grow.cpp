#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    char* ptr = static_cast<char*>(mem::malloc(64));
    ASSERT_NOT_NULL(ptr);

    for (int i = 0; i < 64; i++)
    {
        ptr[i] = static_cast<char>(i);
    }

    char* ptr_grow = static_cast<char*>(mem::realloc(ptr, 128));
    ASSERT_NOT_NULL(ptr_grow);

    for (int i = 0; i < 64; i++)
    {
        ASSERT_TRUE(ptr_grow[i] == static_cast<char>(i));
    }

    for (int i = 64; i < 128; i++)
    {
        ptr_grow[i] = 'X';
    }

    char* ptr_shrink = static_cast<char*>(mem::realloc(ptr_grow, 32));
    ASSERT_NOT_NULL(ptr_shrink);

    for (int i = 0; i < 32; i++)
    {
        ASSERT_TRUE(ptr_shrink[i] == static_cast<char>(i));
    }

    char* ptr_same = static_cast<char*>(mem::realloc(ptr_shrink, 32));
    ASSERT_NOT_NULL(ptr_same);

    for (int i = 0; i < 32; i++)
    {
        ASSERT_TRUE(ptr_same[i] == static_cast<char>(i));
    }

    mem::free(ptr_same);

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
