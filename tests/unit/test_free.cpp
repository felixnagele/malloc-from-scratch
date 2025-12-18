#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    mem::free(nullptr);

    TEST_PASS();
}
