#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>

int main()
{
    constexpr int num_iterations = 300;

    for (int i = 0; i < num_iterations; i++)
    {
        size_t initial_size = 32 + (i % 64);
        void* ptr = mem::malloc(initial_size);
        ASSERT_NOT_NULL(ptr);

        std::memset(ptr, i & 0xFF, initial_size);

        size_t new_size = initial_size * 2;
        void* new_ptr = mem::realloc(ptr, new_size);
        ASSERT_NOT_NULL(new_ptr);

        unsigned char* data = static_cast<unsigned char*>(new_ptr);
        for (size_t j = 0; j < initial_size; j++)
        {
            ASSERT_TRUE(data[j] == static_cast<unsigned char>(i & 0xFF));
        }

        void* final = mem::realloc(new_ptr, initial_size / 2);
        ASSERT_NOT_NULL(final);

        data = static_cast<unsigned char*>(final);
        for (size_t j = 0; j < initial_size / 2; j++)
        {
            ASSERT_TRUE(data[j] == static_cast<unsigned char>(i & 0xFF));
        }

        mem::free(final);
    }

    char* ptr = static_cast<char*>(mem::malloc(64));
    ASSERT_NOT_NULL(ptr);

    for (int i = 0; i < 64; i++)
    {
        ptr[i] = static_cast<char>(i);
    }

    ptr = static_cast<char*>(mem::realloc(ptr, 128));
    ASSERT_NOT_NULL(ptr);
    for (int i = 0; i < 64; i++)
    {
        ASSERT_TRUE(ptr[i] == static_cast<char>(i));
    }

    ptr = static_cast<char*>(mem::realloc(ptr, 32));
    ASSERT_NOT_NULL(ptr);
    for (int i = 0; i < 32; i++)
    {
        ASSERT_TRUE(ptr[i] == static_cast<char>(i));
    }

    ptr = static_cast<char*>(mem::realloc(ptr, 256));
    ASSERT_NOT_NULL(ptr);
    for (int i = 0; i < 32; i++)
    {
        ASSERT_TRUE(ptr[i] == static_cast<char>(i));
    }

    mem::free(ptr);

    ASSERT_TRUE(getTotalUsedMemory() == 0);
    TEST_PASS();
}
