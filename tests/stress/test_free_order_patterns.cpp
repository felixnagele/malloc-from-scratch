#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

void run_free_order_cycle(bool fifo)
{
    for (int cycle = 0; cycle < 100; cycle++)
    {
        void* ptrs[10];
        for (int i = 0; i < 10; i++)
        {
            size_t size = static_cast<size_t>(i + 1) * 16;
            ptrs[i] = mem::malloc(size);
            ASSERT_NOT_NULL(ptrs[i]);
            fill_bytes(ptrs[i], size, static_cast<unsigned char>(i));
        }

        if (fifo)
        {
            for (int i = 0; i < 10; i++)
            {
                mem::free(ptrs[i]);
            }
        }
        else
        {
            for (int i = 9; i >= 0; --i)
            {
                mem::free(ptrs[i]);
            }
        }

        ASSERT_TRUE(getTotalUsedMemory() == 0);
    }
}

int main()
{
    run_free_order_cycle(true);
    run_free_order_cycle(false);

    TEST_PASS();
}
