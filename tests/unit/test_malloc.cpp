#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    void* ptr = mem::malloc(64);
    ASSERT_NOT_NULL(ptr);

    TEST_PASS();
}
