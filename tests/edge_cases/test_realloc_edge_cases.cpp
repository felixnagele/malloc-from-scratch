#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    void* ptr1 = mem::realloc(nullptr, 0);
    (void)ptr1;

    void* ptr2 = mem::malloc(64);
    ASSERT_NOT_NULL(ptr2);

    char* data = static_cast<char*>(ptr2);
    for (int i = 0; i < 64; i++)
    {
        data[i] = static_cast<char>(i);
    }

    void* ptr3 = mem::realloc(ptr2, 0);
    (void)ptr3;

    void* ptr4 = mem::realloc(nullptr, 128);
    ASSERT_NOT_NULL(ptr4);

    mem::free(ptr4);

    TEST_PASS();
}
