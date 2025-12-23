#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int num_iterations = 20;

    for (int i = 0; i < num_iterations; i++)
    {
        void* ptr = mem::malloc(100);
        ASSERT_NOT_NULL(ptr);

        std::memset(ptr, 0xAB, 100);

        unsigned char* data = static_cast<unsigned char*>(ptr);
        for (int j = 0; j < 100; j++)
        {
            ASSERT_TRUE(data[j] == 0xAB);
        }

        mem::free(ptr);

        void* ptr2 = mem::calloc(25, sizeof(int));
        ASSERT_NOT_NULL(ptr2);

        int* int_data = static_cast<int*>(ptr2);
        for (int j = 0; j < 25; j++)
        {
            ASSERT_TRUE(int_data[j] == 0);
        }

        for (int j = 0; j < 25; j++)
        {
            int_data[j] = j * 2;
        }

        mem::free(ptr2);

        void* ptr3 = mem::malloc(50);
        ASSERT_NOT_NULL(ptr3);

        std::memset(ptr3, 'X', 50);

        void* ptr4 = mem::realloc(ptr3, 200);
        ASSERT_NOT_NULL(ptr4);

        char* char_data = static_cast<char*>(ptr4);
        for (int j = 0; j < 50; j++)
        {
            ASSERT_TRUE(char_data[j] == 'X');
        }

        mem::free(ptr4);
    }

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
