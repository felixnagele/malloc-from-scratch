#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    run_in_child(255,
                 []
                 {
                     char* ptr = static_cast<char*>(mem::malloc(64));
                     ASSERT_NOT_NULL(ptr);

                     char* interior_ptr = ptr + 16;
                     // NOLINTNEXTLINE - Intentionally testing interior pointer
                     mem::free(interior_ptr);
                 });

    TEST_PASS();
}
