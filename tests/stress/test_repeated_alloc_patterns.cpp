#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int iterations = 200;
    constexpr int max_blocks = 100;

    char* pointers[max_blocks];
    size_t sizes[max_blocks];

    for (int iteration = 0; iteration < iterations; iteration++)
    {
        for (int i = 0; i < max_blocks; i++)
        {
            size_t size = (i + 1) * (iteration % 8 + 1);
            sizes[i] = size;
            pointers[i] = static_cast<char*>(mem::malloc(size));
            ASSERT_NOT_NULL(pointers[i]);

            for (size_t j = 0; j < size; j++)
            {
                pointers[i][j] = static_cast<char>((i + j + iteration) & 0x7F);
            }
        }

        for (int i = 1; i < max_blocks; i += 3)
        {
            mem::free(pointers[i]);
            pointers[i] = nullptr;
        }

        for (int i = 0; i < max_blocks; i++)
        {
            if (pointers[i] == nullptr)
            {
                continue;
            }

            size_t size = sizes[i];
            for (size_t j = 0; j < size; j++)
            {
                ASSERT_TRUE(pointers[i][j] == static_cast<char>((i + j + iteration) & 0x7F));
            }
        }

        for (int i = 0; i < max_blocks; i++)
        {
            if (pointers[i] != nullptr)
            {
                mem::free(pointers[i]);
                pointers[i] = nullptr;
            }
        }

        std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
        ASSERT_TRUE(getTotalUsedMemory() == 0);
    }

    TEST_PASS();
}
