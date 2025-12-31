#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <pthread.h>
#include <vector>

constexpr int num_threads = 100;
constexpr int allocs_per_thread = 100;

void* thread_alloc_free(void* arg)
{
    for (int i = 0; i < allocs_per_thread; i++)
    {
        size_t size = 64 + (i % 256);
        void* ptr = mem::malloc(size);

        if (ptr == nullptr)
        {
            return reinterpret_cast<void*>(-1);
        }

        char* data = static_cast<char*>(ptr);
        for (size_t j = 0; j < size; j++)
        {
            data[j] = static_cast<char>(i & 0xFF);
        }

        mem::free(ptr);
    }

    return nullptr;
}

int main()
{
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++)
    {
        int result = pthread_create(&threads[i], nullptr, thread_alloc_free, nullptr);
        ASSERT_TRUE(result == 0);
    }

    for (int i = 0; i < num_threads; i++)
    {
        void* thread_result;
        pthread_join(threads[i], &thread_result);

        ASSERT_TRUE(thread_result == nullptr);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
