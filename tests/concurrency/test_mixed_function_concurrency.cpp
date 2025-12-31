#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>
#include <pthread.h>

constexpr int num_malloc_threads = 33;
constexpr int num_calloc_threads = 33;
constexpr int num_realloc_threads = 34;
constexpr int operations_per_thread = 100;

void* thread_malloc_only(void* arg)
{
    for (int i = 0; i < operations_per_thread; i++)
    {
        size_t size = 64 + (i * 4);
        void* ptr = mem::malloc(size);
        if (ptr)
        {
            std::memset(ptr, 'M', size);
            mem::free(ptr);
        }
    }
    return nullptr;
}

void* thread_calloc_only(void* arg)
{
    for (int i = 0; i < operations_per_thread; i++)
    {
        void* ptr = mem::calloc(10, 8 + i);
        if (ptr)
        {
            mem::free(ptr);
        }
    }
    return nullptr;
}

void* thread_realloc_only(void* arg)
{
    for (int i = 0; i < operations_per_thread; i++)
    {
        void* ptr = mem::malloc(32);
        if (ptr)
        {
            for (int j = 0; j < 5; j++)
            {
                void* new_ptr = mem::realloc(ptr, 32 * (j + 2));
                if (new_ptr)
                {
                    ptr = new_ptr;
                }
                else
                {
                    break;
                }
            }
            mem::free(ptr);
        }
    }
    return nullptr;
}

int main()
{
    pthread_t malloc_threads[num_malloc_threads];
    pthread_t calloc_threads[num_calloc_threads];
    pthread_t realloc_threads[num_realloc_threads];

    for (int i = 0; i < num_malloc_threads; i++)
    {
        pthread_create(&malloc_threads[i], nullptr, thread_malloc_only, nullptr);
    }

    for (int i = 0; i < num_calloc_threads; i++)
    {
        pthread_create(&calloc_threads[i], nullptr, thread_calloc_only, nullptr);
    }

    for (int i = 0; i < num_realloc_threads; i++)
    {
        pthread_create(&realloc_threads[i], nullptr, thread_realloc_only, nullptr);
    }

    for (int i = 0; i < num_malloc_threads; i++)
    {
        pthread_join(malloc_threads[i], nullptr);
    }

    for (int i = 0; i < num_calloc_threads; i++)
    {
        pthread_join(calloc_threads[i], nullptr);
    }

    for (int i = 0; i < num_realloc_threads; i++)
    {
        pthread_join(realloc_threads[i], nullptr);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
