#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    void* ptr_8 = mem::malloc(8);
    void* ptr_16 = mem::malloc(16);
    void* ptr_32 = mem::malloc(32);
    void* ptr_64 = mem::malloc(64);
    void* ptr_128 = mem::malloc(128);
    void* ptr_256 = mem::malloc(256);
    void* ptr_512 = mem::malloc(512);
    void* ptr_1k = mem::malloc(1024);
    void* ptr_4k = mem::malloc(4096);

    ASSERT_NOT_NULL(ptr_8);
    ASSERT_NOT_NULL(ptr_16);
    ASSERT_NOT_NULL(ptr_32);
    ASSERT_NOT_NULL(ptr_64);
    ASSERT_NOT_NULL(ptr_128);
    ASSERT_NOT_NULL(ptr_256);
    ASSERT_NOT_NULL(ptr_512);
    ASSERT_NOT_NULL(ptr_1k);
    ASSERT_NOT_NULL(ptr_4k);

    std::memset(ptr_8, 0x11, 8);
    std::memset(ptr_16, 0x22, 16);
    std::memset(ptr_32, 0x33, 32);
    std::memset(ptr_64, 0x44, 64);
    std::memset(ptr_128, 0x55, 128);
    std::memset(ptr_256, 0x66, 256);
    std::memset(ptr_512, 0x77, 512);
    std::memset(ptr_1k, 0x88, 1024);
    std::memset(ptr_4k, 0x99, 4096);

    unsigned char* data_8 = static_cast<unsigned char*>(ptr_8);
    unsigned char* data_16 = static_cast<unsigned char*>(ptr_16);
    unsigned char* data_32 = static_cast<unsigned char*>(ptr_32);

    for (int i = 0; i < 8; i++)
    {
        ASSERT_TRUE(data_8[i] == 0x11);
    }
    for (int i = 0; i < 16; i++)
    {
        ASSERT_TRUE(data_16[i] == 0x22);
    }
    for (int i = 0; i < 32; i++)
    {
        ASSERT_TRUE(data_32[i] == 0x33);
    }

    mem::free(ptr_4k);
    mem::free(ptr_1k);
    mem::free(ptr_512);
    mem::free(ptr_256);
    mem::free(ptr_128);
    mem::free(ptr_64);
    mem::free(ptr_32);
    mem::free(ptr_16);
    mem::free(ptr_8);

    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* p1 = mem::malloc(1);
    void* p2 = mem::malloc(2);
    void* p4 = mem::malloc(4);
    void* p8 = mem::malloc(8);
    void* p16 = mem::malloc(16);

    ASSERT_NOT_NULL(p1);
    ASSERT_NOT_NULL(p2);
    ASSERT_NOT_NULL(p4);
    ASSERT_NOT_NULL(p8);
    ASSERT_NOT_NULL(p16);

    mem::free(p1);
    mem::free(p2);
    mem::free(p4);
    mem::free(p8);
    mem::free(p16);

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
