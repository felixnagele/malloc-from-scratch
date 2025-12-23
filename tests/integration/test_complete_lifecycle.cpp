#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);
    ASSERT_TRUE(getFreeBlockInfo(0) == 0);
    ASSERT_TRUE(getFreeBlockInfo(1) == 0);

    void* block_a = mem::malloc(128);
    void* block_b = mem::malloc(256);
    void* block_c = mem::malloc(512);

    ASSERT_NOT_NULL(block_a);
    ASSERT_NOT_NULL(block_b);
    ASSERT_NOT_NULL(block_c);

    std::cout << "TOTAL MEMORY MID 1: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() >= 128 + 256 + 512);

    std::memset(block_a, 0xAA, 128);
    std::memset(block_b, 0xBB, 256);
    std::memset(block_c, 0xCC, 512);

    mem::free(block_a);
    mem::free(block_b);
    mem::free(block_c);

    std::cout << "TOTAL MEMORY MID 2: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);
    ASSERT_TRUE(getFreeBlockInfo(0) == 0);
    ASSERT_TRUE(getFreeBlockInfo(1) == 0);

    void* block_x = mem::malloc(200);
    void* block_y = mem::malloc(300);

    ASSERT_NOT_NULL(block_x);
    ASSERT_NOT_NULL(block_y);

    std::cout << "TOTAL MEMORY MID 3: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() >= 200 + 300);

    std::memset(block_x, 0x11, 200);
    std::memset(block_y, 0x22, 300);

    unsigned char* data_x = static_cast<unsigned char*>(block_x);
    unsigned char* data_y = static_cast<unsigned char*>(block_y);

    for (int i = 0; i < 200; i++)
    {
        ASSERT_TRUE(data_x[i] == 0x11);
    }
    for (int i = 0; i < 300; i++)
    {
        ASSERT_TRUE(data_y[i] == 0x22);
    }

    mem::free(block_x);
    mem::free(block_y);

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);
    ASSERT_TRUE(getFreeBlockInfo(0) == 0);
    ASSERT_TRUE(getFreeBlockInfo(1) == 0);

    TEST_PASS();
}
