#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    run_in_child(255,
                 []
                 {
                     char* ptr = static_cast<char*>(mem::malloc(64));
                     ASSERT_NOT_NULL(ptr);

                     for (int i = 64; i < 128; i++)
                     {
                         ptr[i] = 'Z';
                     }

                     mem::realloc(ptr, 128);
                 });

    TEST_PASS();
}
