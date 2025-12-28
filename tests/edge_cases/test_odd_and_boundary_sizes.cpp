#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    void* ptr_1 = mem::malloc(1);
    ASSERT_NOT_NULL(ptr_1);

    char* data_1 = static_cast<char*>(ptr_1);
    data_1[0] = 'A';
    ASSERT_TRUE(data_1[0] == 'A');

    void* ptr_3 = mem::malloc(3);
    void* ptr_7 = mem::malloc(7);
    void* ptr_15 = mem::malloc(15);
    void* ptr_31 = mem::malloc(31);

    ASSERT_NOT_NULL(ptr_3);
    ASSERT_NOT_NULL(ptr_7);
    ASSERT_NOT_NULL(ptr_15);
    ASSERT_NOT_NULL(ptr_31);

    std::memset(ptr_3, 'B', 3);
    std::memset(ptr_7, 'C', 7);
    std::memset(ptr_15, 'D', 15);
    std::memset(ptr_31, 'E', 31);

    char* data_3 = static_cast<char*>(ptr_3);
    char* data_7 = static_cast<char*>(ptr_7);

    for (int i = 0; i < 3; i++)
    {
        ASSERT_TRUE(data_3[i] == 'B');
    }
    for (int i = 0; i < 7; i++)
    {
        ASSERT_TRUE(data_7[i] == 'C');
    }

    void* ptr_13 = mem::malloc(13);
    void* ptr_17 = mem::malloc(17);
    void* ptr_23 = mem::malloc(23);
    void* ptr_29 = mem::malloc(29);
    void* ptr_37 = mem::malloc(37);

    ASSERT_NOT_NULL(ptr_13);
    ASSERT_NOT_NULL(ptr_17);
    ASSERT_NOT_NULL(ptr_23);
    ASSERT_NOT_NULL(ptr_29);
    ASSERT_NOT_NULL(ptr_37);

    mem::free(ptr_1);
    mem::free(ptr_3);
    mem::free(ptr_7);
    mem::free(ptr_15);
    mem::free(ptr_31);
    mem::free(ptr_13);
    mem::free(ptr_17);
    mem::free(ptr_23);
    mem::free(ptr_29);
    mem::free(ptr_37);

    void* ptr_63 = mem::malloc(63);
    void* ptr_64 = mem::malloc(64);
    void* ptr_65 = mem::malloc(65);
    void* ptr_127 = mem::malloc(127);
    void* ptr_128 = mem::malloc(128);
    void* ptr_129 = mem::malloc(129);

    ASSERT_NOT_NULL(ptr_63);
    ASSERT_NOT_NULL(ptr_64);
    ASSERT_NOT_NULL(ptr_65);
    ASSERT_NOT_NULL(ptr_127);
    ASSERT_NOT_NULL(ptr_128);
    ASSERT_NOT_NULL(ptr_129);

    mem::free(ptr_63);
    mem::free(ptr_64);
    mem::free(ptr_65);
    mem::free(ptr_127);
    mem::free(ptr_128);
    mem::free(ptr_129);

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
