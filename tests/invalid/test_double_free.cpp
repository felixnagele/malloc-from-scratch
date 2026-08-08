#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    run_in_child(0,
                 []
                 {
                     char* ptr = static_cast<char*>(mem::malloc(16));
                     ASSERT_NOT_NULL(ptr);

                     mem::free(ptr);

                     // NOLINTNEXTLINE - Intentionally testing double free
                     mem::free(ptr);

                     void* fresh = mem::malloc(16);
                     ASSERT_NOT_NULL(fresh);
                     mem::free(fresh);

                     ASSERT_TRUE(getTotalUsedMemory() == 0);
                 });

    TEST_PASS();
}
