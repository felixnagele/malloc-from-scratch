#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    size_t amount = 10;
    void* ptr = mem::calloc(amount, sizeof(int));
    ASSERT_NOT_NULL(ptr);

    int* arr = static_cast<int*>(ptr);
    for (int i = 0; i < amount; i++)
    {
        ASSERT_TRUE(arr[i] == 0);
    }

    TEST_PASS();
}
