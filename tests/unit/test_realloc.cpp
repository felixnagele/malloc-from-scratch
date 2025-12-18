#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    void* null_ptr = mem::realloc(nullptr, 64);
    ASSERT_NOT_NULL(null_ptr);

    TEST_PASS();
}
