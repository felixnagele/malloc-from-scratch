#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstdlib>
#include <cstring>

constexpr int max_blocks = 100;
constexpr int num_iterations = 50;

int main()
{
    int* data[max_blocks];
    size_t sizes[max_blocks];
    unsigned int seed = 42;

    for (int iter = 0; iter < num_iterations; iter++)
    {
        for (int i = 0; i < max_blocks; i++)
        {
            data[i] = nullptr;
            sizes[i] = 0;
        }

        int allocated_count = 0;
        int freed_count = 0;

        for (int op = 0; op < 200; op++)
        {
            int action = rand_r(&seed) % 10;

            if (action >= 3 && allocated_count < max_blocks)
            {
                int idx = allocated_count;
                size_t size = (rand_r(&seed) % 500) + 10;
                sizes[idx] = size;
                data[idx] = static_cast<int*>(mem::malloc(size * sizeof(int)));

                if (data[idx] == nullptr)
                {
                    continue;
                }

                for (size_t j = 0; j < size; j++)
                {
                    data[idx][j] = idx;
                }

                allocated_count = allocated_count + 1;
            }
            else if (allocated_count > freed_count)
            {
                int idx = rand_r(&seed) % allocated_count;

                if (data[idx] == nullptr)
                {
                    continue;
                }

                for (size_t j = 0; j < sizes[idx]; j++)
                {
                    ASSERT_TRUE(data[idx][j] == idx);
                }

                mem::free(data[idx]);
                data[idx] = nullptr;
                freed_count = freed_count + 1;
            }
        }

        for (int i = 0; i < max_blocks; i++)
        {
            if (data[i] != nullptr)
            {
                mem::free(data[i]);
                data[i] = nullptr;
            }
        }

        ASSERT_TRUE(getTotalUsedMemory() == 0);
    }

    TEST_PASS();
}
