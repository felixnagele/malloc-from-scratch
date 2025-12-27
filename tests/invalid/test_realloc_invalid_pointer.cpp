#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    int stack_var = 42;
    // NOLINTNEXTLINE - Intentionally testing invalid pointer
    void* result = mem::realloc(&stack_var, 128);

    ASSERT_NULL(result);

    void* ptr = mem::malloc(64);
    ASSERT_NOT_NULL(ptr);

    mem::free(ptr);

    void* result2 = mem::realloc(ptr, 128);
    ASSERT_NULL(result2);

    TEST_PASS();
}
