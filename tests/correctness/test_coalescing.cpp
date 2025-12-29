#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* ptr1 = mem::malloc(32);
    void* ptr2 = mem::malloc(32);
    void* ptr3 = mem::malloc(32);
    ASSERT_NOT_NULL(ptr1);
    ASSERT_NOT_NULL(ptr2);
    ASSERT_NOT_NULL(ptr3);

    mem::free(ptr2);
    size_t free_size_before = getFreeBlockInfo(1);

    mem::free(ptr1);
    size_t free_size_after = getFreeBlockInfo(1);

    ASSERT_TRUE(free_size_after > free_size_before);

    mem::free(ptr3);

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    for (int iter = 0; iter < 100; iter++)
    {
        void* blocks[5];
        for (int i = 0; i < 5; i++)
        {
            size_t size = 64 + (i * 32);
            blocks[i] = mem::malloc(size);
            ASSERT_NOT_NULL(blocks[i]);
            std::memset(blocks[i], 'A' + i, size);
        }

        mem::free(blocks[1]);
        mem::free(blocks[3]);

        void* new1 = mem::malloc(80);
        void* new2 = mem::malloc(80);
        ASSERT_NOT_NULL(new1);
        ASSERT_NOT_NULL(new2);

        unsigned char* b0 = static_cast<unsigned char*>(blocks[0]);
        unsigned char* b2 = static_cast<unsigned char*>(blocks[2]);
        unsigned char* b4 = static_cast<unsigned char*>(blocks[4]);

        ASSERT_TRUE(b0[0] == 'A');
        ASSERT_TRUE(b2[0] == 'C');
        ASSERT_TRUE(b4[0] == 'E');

        mem::free(blocks[0]);
        mem::free(blocks[2]);
        mem::free(blocks[4]);
        mem::free(new1);
        mem::free(new2);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);
    TEST_PASS();
}
