#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr size_t array_size_0 = 5;
    constexpr size_t array_size_1 = 20;
    constexpr size_t array_size_2 = 40;
    constexpr size_t array_size_3 = 100;
    constexpr size_t array_size_4 = 500;
    constexpr char test_string[] = "A string for testing.";

    char* data[5];

    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    data[0] = static_cast<char*>(mem::malloc(array_size_0 * sizeof(char)));
    ASSERT_NOT_NULL(data[0]);
    std::cout << "TOTAL MEMORY MID 1: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == array_size_0 * sizeof(char));

    data[1] = static_cast<char*>(mem::malloc(array_size_1 * sizeof(char)));
    data[2] = static_cast<char*>(mem::malloc(array_size_2 * sizeof(char)));
    data[3] = static_cast<char*>(mem::malloc(array_size_3 * sizeof(char)));
    data[4] = static_cast<char*>(mem::malloc(array_size_4 * sizeof(char)));

    ASSERT_NOT_NULL(data[1]);
    ASSERT_NOT_NULL(data[2]);
    ASSERT_NOT_NULL(data[3]);
    ASSERT_NOT_NULL(data[4]);

    for (size_t i = 0; i < array_size_2; i++)
    {
        data[2][i] = 'A';
    }

    std::memcpy(data[4], test_string, std::strlen(test_string) + 1);

    size_t total_size =
        (array_size_0 + array_size_1 + array_size_2 + array_size_3 + array_size_4) * sizeof(char);
    std::cout << "TOTAL MEMORY MID 2: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == total_size);

    mem::free(data[0]);
    mem::free(data[1]);
    mem::free(data[3]);

    for (size_t i = 0; i < array_size_2; i++)
    {
        ASSERT_TRUE(data[2][i] == 'A');
    }

    ASSERT_TRUE(std::strcmp(data[4], test_string) == 0);

    data[0] = static_cast<char*>(mem::malloc(25 * sizeof(char)));
    ASSERT_NOT_NULL(data[0]);

    mem::free(data[0]);
    mem::free(data[2]);
    mem::free(data[4]);

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
