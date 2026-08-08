#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    size_t odd_sizes[] = {1, 3, 7, 13, 15, 17, 23, 29, 31, 37, 63, 65, 127, 129};

    for (size_t i = 0; i < sizeof(odd_sizes) / sizeof(odd_sizes[0]); i++)
    {
        void* ptr = mem::malloc(odd_sizes[i]);
        ASSERT_NOT_NULL(ptr);
        std::memset(ptr, 'A' + i, odd_sizes[i]);

        unsigned char* data = static_cast<unsigned char*>(ptr);
        for (size_t j = 0; j < odd_sizes[i]; j++)
        {
            ASSERT_TRUE(data[j] == static_cast<unsigned char>('A' + i));
        }
        mem::free(ptr);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);
    TEST_PASS();
}
