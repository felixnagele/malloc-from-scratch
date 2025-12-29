#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    size_t sizes[] = {8, 16, 32, 64, 128, 256, 512, 1024, 4096};
    void* ptrs[9];

    for (int i = 0; i < 9; i++)
    {
        ptrs[i] = mem::malloc(sizes[i]);
        ASSERT_NOT_NULL(ptrs[i]);
        std::memset(ptrs[i], 0x11 * (i + 1), sizes[i]);
    }

    for (int i = 0; i < 9; i++)
    {
        unsigned char* data = static_cast<unsigned char*>(ptrs[i]);
        ASSERT_TRUE(data[0] == 0x11 * (i + 1));
        ASSERT_TRUE(data[sizes[i] - 1] == 0x11 * (i + 1));
        mem::free(ptrs[i]);
    }

    size_t odd_sizes[] = {1, 3, 7, 13, 15, 17, 23, 29, 31, 37, 63, 65, 127, 129};
    for (int i = 0; i < 14; i++)
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
