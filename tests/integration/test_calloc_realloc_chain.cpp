#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int num_iterations = 300;

    for (int iter = 0; iter < num_iterations; iter++)
    {
        void* ptr1 = mem::malloc(128);
        ASSERT_NOT_NULL(ptr1);
        std::memset(ptr1, 'A', 128);

        void* ptr2 = mem::realloc(ptr1, 256);
        ASSERT_NOT_NULL(ptr2);

        unsigned char* data = static_cast<unsigned char*>(ptr2);
        for (int i = 0; i < 128; i++)
        {
            ASSERT_TRUE(data[i] == 'A');
        }

        void* ptr3 = mem::calloc(50, 8);
        ASSERT_NOT_NULL(ptr3);

        unsigned char* zero_data = static_cast<unsigned char*>(ptr3);
        for (int i = 0; i < 400; i++)
        {
            ASSERT_TRUE(zero_data[i] == 0);
        }

        void* ptr4 = mem::realloc(ptr3, 800);
        ASSERT_NOT_NULL(ptr4);

        zero_data = static_cast<unsigned char*>(ptr4);
        for (int i = 0; i < 400; i++)
        {
            ASSERT_TRUE(zero_data[i] == 0);
        }

        mem::free(ptr2);
        mem::free(ptr4);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
