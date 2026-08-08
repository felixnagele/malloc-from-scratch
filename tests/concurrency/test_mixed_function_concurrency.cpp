#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <pthread.h>

constexpr int num_threads_per_type = 10;
constexpr int operations_per_thread = 100;

void* thread_malloc_only(void* arg)
{
    (void)arg;
    for (int i = 0; i < operations_per_thread; i++)
    {
        size_t size = 64 + (i * 4);
        char* ptr = static_cast<char*>(mem::malloc(size));
        if (ptr == nullptr)
        {
            return reinterpret_cast<void*>(-1);
        }

        fill_bytes(ptr, size, static_cast<unsigned char>(i));
        if (ptr[0] != static_cast<unsigned char>(i) ||
            ptr[size - 1] != static_cast<unsigned char>(i))
        {
            mem::free(ptr);
            return reinterpret_cast<void*>(-1);
        }

        mem::free(ptr);
    }
    return nullptr;
}

void* thread_calloc_only(void* arg)
{
    (void)arg;
    for (int i = 0; i < operations_per_thread; i++)
    {
        void* ptr = mem::calloc(10, 8 + i);
        if (ptr == nullptr)
        {
            return reinterpret_cast<void*>(-1);
        }

        unsigned char* data = static_cast<unsigned char*>(ptr);
        for (int j = 0; j < 10 * (8 + i); j++)
        {
            if (data[j] != 0)
            {
                mem::free(ptr);
                return reinterpret_cast<void*>(-1);
            }
        }

        mem::free(ptr);
    }
    return nullptr;
}

void* thread_realloc_only(void* arg)
{
    (void)arg;
    for (int i = 0; i < operations_per_thread; i++)
    {
        void* ptr = mem::malloc(32);
        if (ptr == nullptr)
        {
            return reinterpret_cast<void*>(-1);
        }
        fill_bytes(ptr, 32, static_cast<unsigned char>(i));

        for (int j = 1; j <= 5; j++)
        {
            void* new_ptr = mem::realloc(ptr, 32 * (j + 1));
            if (new_ptr == nullptr)
            {
                mem::free(ptr);
                return reinterpret_cast<void*>(-1);
            }

            unsigned char* data = static_cast<unsigned char*>(new_ptr);
            for (int k = 0; k < 32; k++)
            {
                if (data[k] != static_cast<unsigned char>(i))
                {
                    mem::free(new_ptr);
                    return reinterpret_cast<void*>(-1);
                }
            }

            ptr = new_ptr;
        }

        mem::free(ptr);
    }
    return nullptr;
}

int main()
{
    pthread_t malloc_threads[num_threads_per_type];
    pthread_t calloc_threads[num_threads_per_type];
    pthread_t realloc_threads[num_threads_per_type];

    for (int i = 0; i < num_threads_per_type; i++)
    {
        ASSERT_TRUE(pthread_create(&malloc_threads[i], nullptr, thread_malloc_only, nullptr) == 0);
    }
    for (int i = 0; i < num_threads_per_type; i++)
    {
        ASSERT_TRUE(pthread_create(&calloc_threads[i], nullptr, thread_calloc_only, nullptr) == 0);
    }
    for (int i = 0; i < num_threads_per_type; i++)
    {
        ASSERT_TRUE(pthread_create(&realloc_threads[i], nullptr, thread_realloc_only, nullptr) ==
                    0);
    }

    for (int i = 0; i < num_threads_per_type; i++)
    {
        void* result = nullptr;
        pthread_join(malloc_threads[i], &result);
        ASSERT_TRUE(result == nullptr);
    }
    for (int i = 0; i < num_threads_per_type; i++)
    {
        void* result = nullptr;
        pthread_join(calloc_threads[i], &result);
        ASSERT_TRUE(result == nullptr);
    }
    for (int i = 0; i < num_threads_per_type; i++)
    {
        void* result = nullptr;
        pthread_join(realloc_threads[i], &result);
        ASSERT_TRUE(result == nullptr);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
