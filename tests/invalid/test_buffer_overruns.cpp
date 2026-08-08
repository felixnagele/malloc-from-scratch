#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    run_in_child(255,
                 []
                 {
                     char* ptr = static_cast<char*>(mem::malloc(128));
                     ASSERT_NOT_NULL(ptr);

                     for (int i = 0; i < 4096; i++)
                     {
                         ptr[i] = static_cast<char>(i & 0xFF);
                     }

                     mem::free(ptr);
                 });

    TEST_PASS();
}
