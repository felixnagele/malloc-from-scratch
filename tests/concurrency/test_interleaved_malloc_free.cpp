#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>
#include <pthread.h>
#include <vector>

constexpr int num_threads = 100;
constexpr int operations_per_thread = 200;

struct SharedAllocations
{
    pthread_mutex_t mutex;
    std::vector<void*> allocations;

    SharedAllocations() { pthread_mutex_init(&mutex, nullptr); }
    ~SharedAllocations() { pthread_mutex_destroy(&mutex); }
};

void* thread_interleaved_ops(void* arg)
{
    SharedAllocations* shared = static_cast<SharedAllocations*>(arg);

    for (int i = 0; i < operations_per_thread; i++)
    {
        if (i % 2 == 0)
        {
            size_t size = 32 + (i % 128);
            void* ptr = mem::malloc(size);
            if (ptr)
            {
                std::memset(ptr, i & 0xFF, size);

                pthread_mutex_lock(&shared->mutex);
                shared->allocations.push_back(ptr);
                pthread_mutex_unlock(&shared->mutex);
            }
        }
        else
        {
            void* ptr = nullptr;

            pthread_mutex_lock(&shared->mutex);
            if (!shared->allocations.empty())
            {
                ptr = shared->allocations.back();
                shared->allocations.pop_back();
            }
            pthread_mutex_unlock(&shared->mutex);

            if (ptr)
            {
                mem::free(ptr);
            }
        }
    }

    return nullptr;
}

int main()
{
    SharedAllocations shared;
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++)
    {
        int result = pthread_create(&threads[i], nullptr, thread_interleaved_ops, &shared);
        ASSERT_TRUE(result == 0);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], nullptr);
    }

    for (void* ptr : shared.allocations)
    {
        mem::free(ptr);
    }

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
