#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <cstring>
#include <pthread.h>

constexpr int num_threads = 100;
constexpr int reallocs_per_thread = 50;

void* thread_realloc_stress(void* arg)
{
    int thread_id = *static_cast<int*>(arg);

    for (int i = 0; i < reallocs_per_thread; i++)
    {
        size_t initial_size = 16 + (thread_id % 32);
        void* ptr = mem::malloc(initial_size);
        if (ptr == nullptr)
        {
            return reinterpret_cast<void*>(-1);
        }

        std::memset(ptr, thread_id & 0xFF, initial_size);

        for (int j = 0; j < 5; j++)
        {
            size_t new_size = initial_size * (j + 2);
            void* new_ptr = mem::realloc(ptr, new_size);
            if (new_ptr == nullptr)
            {
                mem::free(ptr);
                return reinterpret_cast<void*>(-1);
            }

            unsigned char* data = static_cast<unsigned char*>(new_ptr);
            for (size_t k = 0; k < initial_size; k++)
            {
                if (data[k] != static_cast<unsigned char>(thread_id & 0xFF))
                {
                    mem::free(new_ptr);
                    return reinterpret_cast<void*>(-1);
                }
            }

            ptr = new_ptr;
            initial_size = new_size;

            std::memset(ptr, thread_id & 0xFF, new_size);
        }

        for (int j = 0; j < 3; j++)
        {
            size_t new_size = initial_size / 2;
            if (new_size < 8)
                new_size = 8;

            void* new_ptr = mem::realloc(ptr, new_size);
            if (new_ptr == nullptr)
            {
                mem::free(ptr);
                return reinterpret_cast<void*>(-1);
            }

            ptr = new_ptr;
            initial_size = new_size;
        }

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
        int result = pthread_create(&threads[i], nullptr, thread_realloc_stress, &thread_ids[i]);
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
