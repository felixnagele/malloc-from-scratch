#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int num_strings = 30;
    constexpr int string_length = 64;
    constexpr char test_text[] = "Hello custom malloc!";

    char* strings[num_strings];

    std::cout << "TOTAL MEMORY START: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    for (int i = 0; i < num_strings; i++)
    {
        if (i < num_strings / 2)
        {
            strings[i] = static_cast<char*>(mem::calloc(string_length, sizeof(char)));
        }
        else
        {
            strings[i] = static_cast<char*>(mem::malloc(string_length));
            ASSERT_NOT_NULL(strings[i]);
            std::memset(strings[i], 0, string_length);
        }
        ASSERT_NOT_NULL(strings[i]);
    }

    for (int i = 0; i < num_strings / 2; i++)
    {
        for (int j = 0; j < string_length; j++)
        {
            ASSERT_TRUE(strings[i][j] == 0);
        }
    }

    for (int i = 0; i < num_strings; i++)
    {
        std::strncpy(strings[i], test_text, string_length - 1);
        strings[i][string_length - 1] = '\0';
    }

    for (int i = 0; i < num_strings; i++)
    {
        ASSERT_TRUE(std::strcmp(strings[i], test_text) == 0);
    }

    size_t memory_used = getTotalUsedMemory();
    std::cout << "TOTAL MEMORY MID: " << memory_used << std::endl;
    ASSERT_TRUE(memory_used >= num_strings * string_length);

    for (int i = 0; i < num_strings; i += 3)
    {
        mem::free(strings[i]);
        strings[i] = nullptr;
    }

    for (int i = 0; i < 10; i++)
    {
        char* temp = static_cast<char*>(mem::malloc(32));
        ASSERT_NOT_NULL(temp);
        std::memset(temp, 'X', 32);
        mem::free(temp);
    }

    for (int i = 0; i < num_strings; i++)
    {
        if (strings[i] != nullptr)
        {
            mem::free(strings[i]);
        }
    }

    std::cout << "TOTAL MEMORY END: " << getTotalUsedMemory() << std::endl;
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
