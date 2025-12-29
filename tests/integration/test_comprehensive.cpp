#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* malloc_ptr = mem::malloc(64);
    ASSERT_NOT_NULL(malloc_ptr);
    std::memset(malloc_ptr, 0xAB, 64);
    ASSERT_TRUE(getTotalUsedMemory() >= 64);
    mem::free(malloc_ptr);
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    int* calloc_array = static_cast<int*>(mem::calloc(10, sizeof(int)));
    ASSERT_NOT_NULL(calloc_array);
    for (int i = 0; i < 10; i++)
    {
        ASSERT_TRUE(calloc_array[i] == 0);
    }
    mem::free(calloc_array);
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* realloc_ptr = mem::realloc(nullptr, 128);
    ASSERT_NOT_NULL(realloc_ptr);

    char* data = static_cast<char*>(realloc_ptr);
    for (int i = 0; i < 128; i++)
    {
        data[i] = static_cast<char>(i);
    }

    void* grown = mem::realloc(realloc_ptr, 256);
    ASSERT_NOT_NULL(grown);
    data = static_cast<char*>(grown);
    for (int i = 0; i < 128; i++)
    {
        ASSERT_TRUE(data[i] == static_cast<char>(i));
    }
    mem::free(grown);

    ASSERT_NULL(mem::malloc(0));
    ASSERT_NULL(mem::calloc(0, 10));
    ASSERT_NULL(mem::calloc(10, 0));

    for (int i = 0; i < 20; i++)
    {
        void* ptr;
        if (i % 3 == 0)
        {
            ptr = mem::malloc(100);
        }
        else if (i % 3 == 1)
        {
            ptr = mem::calloc(25, sizeof(int));
        }
        else
        {
            void* temp = mem::malloc(50);
            ptr = mem::realloc(temp, 200);
        }
        ASSERT_NOT_NULL(ptr);
        mem::free(ptr);
    }

    constexpr int num_strings = 20;
    char* strings[num_strings];

    for (int i = 0; i < num_strings; i++)
    {
        if (i < num_strings / 2)
        {
            strings[i] = static_cast<char*>(mem::calloc(64, sizeof(char)));
        }
        else
        {
            strings[i] = static_cast<char*>(mem::malloc(64));
            std::memset(strings[i], 0, 64);
        }
        ASSERT_NOT_NULL(strings[i]);
        strings[i][0] = 'A' + i;
    }

    for (int i = 0; i < num_strings; i++)
    {
        ASSERT_TRUE(strings[i][0] == 'A' + i);
        mem::free(strings[i]);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);
    TEST_PASS();
}
