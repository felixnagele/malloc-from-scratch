#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    void* malloc_ptr = mem::malloc(64);
    ASSERT_NOT_NULL(malloc_ptr);
    std::memset(malloc_ptr, 0xAB, 64);

    size_t used_after_malloc = getTotalUsedMemory();
    std::cout << "TOTAL MEMORY START: " << used_after_malloc << std::endl;
    ASSERT_TRUE(used_after_malloc >= 64);

    mem::free(malloc_ptr);
    std::cout << "TOTAL MEMORY MID 1: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    int* calloc_array = static_cast<int*>(mem::calloc(10, sizeof(int)));
    ASSERT_NOT_NULL(calloc_array);

    for (int i = 0; i < 10; i++)
    {
        ASSERT_TRUE(calloc_array[i] == 0);
    }

    mem::free(calloc_array);
    std::cout << "TOTAL MEMORY MID 2: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    mem::free(nullptr);
    std::cout << "TOTAL MEMORY MID 3: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* realloc_ptr = mem::realloc(nullptr, 128);
    ASSERT_NOT_NULL(realloc_ptr);

    char* realloc_data = static_cast<char*>(realloc_ptr);
    for (int i = 0; i < 128; i++)
    {
        realloc_data[i] = static_cast<char>(i);
    }

    void* grown_ptr = mem::realloc(realloc_ptr, 256);
    ASSERT_NOT_NULL(grown_ptr);

    char* grown_data = static_cast<char*>(grown_ptr);
    for (int i = 0; i < 128; i++)
    {
        ASSERT_TRUE(grown_data[i] == static_cast<char>(i));
    }

    mem::free(grown_ptr);

    void* zero_malloc = mem::malloc(0);
    void* zero_calloc1 = mem::calloc(0, 10);
    void* zero_calloc2 = mem::calloc(10, 0);

    ASSERT_NULL(zero_malloc);
    ASSERT_NULL(zero_calloc1);
    ASSERT_NULL(zero_calloc2);

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
