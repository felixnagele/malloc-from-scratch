#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

void check_pattern(void* ptr, size_t size, unsigned char base)
{
    unsigned char* data = static_cast<unsigned char*>(ptr);
    for (size_t i = 0; i < size; i++)
    {
        ASSERT_TRUE(data[i] == static_cast<unsigned char>(base + (i & 0xFF)));
    }
}

int main()
{
    constexpr int num_iterations = 300;

    for (int i = 0; i < num_iterations; i++)
    {
        size_t initial_size = 32 + (i % 64);
        unsigned char pattern = static_cast<unsigned char>(i);

        void* ptr = mem::malloc(initial_size);
        ASSERT_NOT_NULL(ptr);
        fill_bytes(ptr, initial_size, pattern);

        void* new_ptr = mem::realloc(ptr, initial_size * 2);
        ASSERT_NOT_NULL(new_ptr);
        check_bytes(new_ptr, initial_size, pattern);

        void* final = mem::realloc(new_ptr, initial_size / 2);
        ASSERT_NOT_NULL(final);
        check_bytes(final, initial_size / 2, pattern);

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
    check_pattern(ptr, 64, 0);

    ptr = static_cast<char*>(mem::realloc(ptr, 32));
    ASSERT_NOT_NULL(ptr);
    check_pattern(ptr, 32, 0);

    ptr = static_cast<char*>(mem::realloc(ptr, 256));
    ASSERT_NOT_NULL(ptr);
    check_pattern(ptr, 32, 0);

    ptr = static_cast<char*>(mem::realloc(ptr, 256));
    ASSERT_NOT_NULL(ptr);
    check_pattern(ptr, 32, 0);

    mem::free(ptr);

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* small = mem::malloc(64);
    ASSERT_NOT_NULL(small);
    ASSERT_TRUE(getTotalUsedMemory() == 64);
    ASSERT_NULL(mem::realloc(small, 0));
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
