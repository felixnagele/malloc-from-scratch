#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

#include <cstring>

int main()
{
    constexpr int num_pointers = 500;
    char* pointers[num_pointers];

    for (int i = 0; i < num_pointers; i++)
    {
        pointers[i] = static_cast<char*>(mem::malloc(32));
        ASSERT_NOT_NULL(pointers[i]);
        pointers[i][0] = static_cast<char>(i);
    }

    ASSERT_TRUE(getTotalUsedMemory() >= num_pointers * 32);

    for (int i = 0; i < num_pointers; i += 2)
    {
        mem::free(pointers[i]);
        pointers[i] = nullptr;
    }

    for (int i = 1; i < num_pointers; i += 2)
    {
        ASSERT_TRUE(pointers[i][0] == static_cast<char>(i));
        mem::free(pointers[i]);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);
    TEST_PASS();
}
