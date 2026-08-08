#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* a = mem::malloc(32);
    void* b = mem::malloc(32);
    void* c = mem::malloc(32);
    void* d = mem::malloc(32);
    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(b);
    ASSERT_NOT_NULL(c);
    ASSERT_NOT_NULL(d);

    fill_bytes(a, 32, 'A');
    fill_bytes(b, 32, 'B');
    fill_bytes(c, 32, 'C');
    fill_bytes(d, 32, 'D');

    mem::free(b);
    size_t single_block_size = getFreeBlockInfo(1);
    ASSERT_TRUE(single_block_size == 32);

    mem::free(c);
    size_t merged_block_size = getFreeBlockInfo(1);
    ASSERT_TRUE(merged_block_size > single_block_size);

    check_bytes(a, 32, 'A');
    check_bytes(d, 32, 'D');

    void* reused = mem::malloc(64);
    ASSERT_NOT_NULL(reused);
    fill_bytes(reused, 64, 'X');
    check_bytes(reused, 64, 'X');

    mem::free(reused);
    mem::free(a);
    mem::free(d);

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    void* x = mem::malloc(32);
    void* y = mem::malloc(32);
    void* z = mem::malloc(32);
    ASSERT_NOT_NULL(x);
    ASSERT_NOT_NULL(y);
    ASSERT_NOT_NULL(z);
    fill_bytes(x, 32, 'X');
    fill_bytes(y, 32, 'Y');
    fill_bytes(z, 32, 'Z');

    mem::free(y);
    mem::free(z);

    ASSERT_TRUE(getTotalUsedMemory() == 32);
    check_bytes(x, 32, 'X');

    mem::free(x);
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
