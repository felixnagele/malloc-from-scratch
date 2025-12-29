#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    void* ptr1 = mem::realloc(nullptr, 64);
    ASSERT_NOT_NULL(ptr1);
    std::memset(ptr1, 'A', 64);

    void* ptr2 = mem::realloc(ptr1, 0);
    (void)ptr2;

    void* ptr3 = mem::malloc(100);
    ASSERT_NOT_NULL(ptr3);
    std::memset(ptr3, 'B', 100);

    void* ptr4 = mem::realloc(ptr3, 100);
    ASSERT_NOT_NULL(ptr4);
    unsigned char* data = static_cast<unsigned char*>(ptr4);
    for (int i = 0; i < 100; i++)
    {
        ASSERT_TRUE(data[i] == 'B');
    }

    void* ptr5 = mem::realloc(ptr4, 50);
    ASSERT_NOT_NULL(ptr5);
    data = static_cast<unsigned char*>(ptr5);
    for (int i = 0; i < 50; i++)
    {
        ASSERT_TRUE(data[i] == 'B');
    }

    void* ptr6 = mem::realloc(ptr5, 10);
    ASSERT_NOT_NULL(ptr6);
    data = static_cast<unsigned char*>(ptr6);
    for (int i = 0; i < 10; i++)
    {
        ASSERT_TRUE(data[i] == 'B');
    }

    void* ptr7 = mem::realloc(ptr6, 200);
    ASSERT_NOT_NULL(ptr7);
    data = static_cast<unsigned char*>(ptr7);
    for (int i = 0; i < 10; i++)
    {
        ASSERT_TRUE(data[i] == 'B');
    }

    mem::free(ptr7);

    for (size_t size = 16; size <= 4096; size *= 2)
    {
        void* ptr = mem::malloc(size);
        ASSERT_NOT_NULL(ptr);
        std::memset(ptr, size & 0xFF, size);

        void* same = mem::realloc(ptr, size);
        ASSERT_NOT_NULL(same);
        ASSERT_TRUE(static_cast<unsigned char*>(same)[0] == (size & 0xFF));

        mem::free(same);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);
    TEST_PASS();
}
