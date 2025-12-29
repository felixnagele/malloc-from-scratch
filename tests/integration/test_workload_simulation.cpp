#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>
#include <vector>

int main()
{
    constexpr int num_iterations = 100;

    for (int iter = 0; iter < num_iterations; iter++)
    {
        std::vector<void*> small_allocs;
        std::vector<void*> medium_allocs;
        std::vector<void*> large_allocs;

        for (int i = 0; i < 20; i++)
        {
            void* ptr = mem::malloc(16 + (i % 8));
            ASSERT_NOT_NULL(ptr);
            std::memset(ptr, 'S', 16 + (i % 8));
            small_allocs.push_back(ptr);
        }

        for (int i = 0; i < 10; i++)
        {
            void* ptr = mem::malloc(256 + (i * 32));
            ASSERT_NOT_NULL(ptr);
            std::memset(ptr, 'M', 256 + (i * 32));
            medium_allocs.push_back(ptr);
        }

        for (int i = 0; i < 5; i++)
        {
            void* ptr = mem::malloc(2048 + (i * 512));
            ASSERT_NOT_NULL(ptr);
            std::memset(ptr, 'L', 2048 + (i * 512));
            large_allocs.push_back(ptr);
        }

        void* zero_mem = mem::calloc(100, 4);
        ASSERT_NOT_NULL(zero_mem);
        unsigned char* zero_data = static_cast<unsigned char*>(zero_mem);
        for (int i = 0; i < 400; i++)
        {
            ASSERT_TRUE(zero_data[i] == 0);
        }

        for (int i = 0; i < 5; i++)
        {
            void* new_ptr = mem::realloc(medium_allocs[i], 512);
            ASSERT_NOT_NULL(new_ptr);
            medium_allocs[i] = new_ptr;
        }

        for (int i = 0; i < 5; i++)
        {
            mem::free(small_allocs[i * 4]);
        }

        for (void* ptr : large_allocs)
        {
            mem::free(ptr);
        }

        mem::free(zero_mem);

        for (void* ptr : medium_allocs)
        {
            mem::free(ptr);
        }

        for (void* ptr : small_allocs)
        {
            if (ptr)
                mem::free(ptr);
        }
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
