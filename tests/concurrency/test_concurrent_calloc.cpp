#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>
#include <pthread.h>

constexpr int num_threads = 100;
constexpr int callocs_per_thread = 100;

void* thread_calloc_operations(void* arg)
{
    int thread_id = *static_cast<int*>(arg);

    for (int i = 0; i < callocs_per_thread; i++)
    {
        size_t count = 10 + (i % 50);
        size_t element_size = sizeof(int) + (thread_id % 16);

        void* ptr = mem::calloc(count, element_size);
        if (ptr == nullptr)
        {
            return reinterpret_cast<void*>(-1);
        }

        unsigned char* data = static_cast<unsigned char*>(ptr);
        for (size_t j = 0; j < count * element_size; j++)
        {
            if (data[j] != 0)
            {
                mem::free(ptr);
                return reinterpret_cast<void*>(-1);
            }
        }

        std::memset(ptr, thread_id & 0xFF, count * element_size);

        mem::free(ptr);
    }

    return nullptr;
}

int main()
{
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    for (int i = 0; i < num_threads; i++)
    {
        thread_ids[i] = i;
        int result = pthread_create(&threads[i], nullptr, thread_calloc_operations, &thread_ids[i]);
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
