#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    char* ptr = static_cast<char*>(mem::malloc(16));
    ASSERT_NOT_NULL(ptr);

    mem::free(ptr);

    // NOLINTNEXTLINE - Intentionally testing double free
    mem::free(ptr);

    TEST_PASS();
}
