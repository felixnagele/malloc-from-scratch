#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>
#include <pthread.h>

constexpr int num_threads = 100;
constexpr int blocks_per_thread = 50;

struct ThreadData
{
    int thread_id;
    void* blocks[blocks_per_thread];
};

void* thread_allocate(void* arg)
{
    ThreadData* data = static_cast<ThreadData*>(arg);

    for (int i = 0; i < blocks_per_thread; i++)
    {
        size_t size = 32 + (data->thread_id * 16) + (i * 8);
        data->blocks[i] = mem::malloc(size);

        if (data->blocks[i] == nullptr)
        {
            return reinterpret_cast<void*>(-1);
        }

        std::memset(data->blocks[i], data->thread_id & 0xFF, size);
    }

    return nullptr;
}

void* thread_free(void* arg)
{
    ThreadData* data = static_cast<ThreadData*>(arg);

    for (int i = 0; i < blocks_per_thread; i++)
    {
        if (data->blocks[i] != nullptr)
        {
            mem::free(data->blocks[i]);
            data->blocks[i] = nullptr;
        }
    }

    return nullptr;
}

int main()
{
    pthread_t alloc_threads[num_threads];
    pthread_t free_threads[num_threads];
    ThreadData thread_data[num_threads];

    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].thread_id = i;
        for (int j = 0; j < blocks_per_thread; j++)
        {
            thread_data[i].blocks[j] = nullptr;
        }
    }

    for (int i = 0; i < num_threads; i++)
    {
        int result = pthread_create(&alloc_threads[i], nullptr, thread_allocate, &thread_data[i]);
        ASSERT_TRUE(result == 0);
    }

    for (int i = 0; i < num_threads; i++)
    {
        void* result;
        pthread_join(alloc_threads[i], &result);
        ASSERT_TRUE(result == nullptr);
    }

    for (int i = 0; i < num_threads; i++)
    {
        int result = pthread_create(&free_threads[i], nullptr, thread_free, &thread_data[i]);
        ASSERT_TRUE(result == 0);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(free_threads[i], nullptr);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
