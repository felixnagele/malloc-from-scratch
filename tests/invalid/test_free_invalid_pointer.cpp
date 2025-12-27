#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    int stack_variable = 42;

    // NOLINTNEXTLINE - Intentionally testing invalid pointer
    mem::free(&stack_variable);

    TEST_PASS();
}
