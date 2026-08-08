#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"

int main()
{
    ASSERT_TRUE(getTotalUsedMemory() == 0);

    char* a = static_cast<char*>(mem::malloc(5));
    char* b = static_cast<char*>(mem::malloc(20));
    char* c = static_cast<char*>(mem::malloc(40));
    char* d = static_cast<char*>(mem::malloc(100));
    char* e = static_cast<char*>(mem::malloc(500));
    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(b);
    ASSERT_NOT_NULL(c);
    ASSERT_NOT_NULL(d);
    ASSERT_NOT_NULL(e);

    ASSERT_TRUE(getTotalUsedMemory() == 665); // 5 + 20 + 40 + 100 + 500

    fill_bytes(a, 5, 'A');
    fill_bytes(b, 20, 'B');
    fill_bytes(c, 40, 'C');
    fill_bytes(d, 100, 'D');
    fill_bytes(e, 500, 'E');

    mem::free(b);
    mem::free(d);
    ASSERT_TRUE(getTotalUsedMemory() == 545); // 665 - 20 - 100

    check_bytes(a, 5, 'A');
    check_bytes(c, 40, 'C');
    check_bytes(e, 500, 'E');

    // size difference (400 - 40).
    char* grown = static_cast<char*>(mem::realloc(c, 400));
    ASSERT_NOT_NULL(grown);
    check_bytes(grown, 40, 'C');
    ASSERT_TRUE(getTotalUsedMemory() == 905); // 545 - 40 + 400

    void* same = mem::realloc(grown, 400);
    ASSERT_TRUE(same == grown);
    ASSERT_TRUE(getTotalUsedMemory() == 905);

    void* shrunk = mem::realloc(grown, 20);
    ASSERT_TRUE(shrunk == grown);
    check_bytes(shrunk, 20, 'C');
    ASSERT_TRUE(getTotalUsedMemory() == 905);

    char* reuse = static_cast<char*>(mem::malloc(20));
    ASSERT_NOT_NULL(reuse);
    ASSERT_TRUE(getTotalUsedMemory() == 925); // 905 + 20

    check_bytes(a, 5, 'A');
    check_bytes(e, 500, 'E');

    mem::free(a);
    mem::free(shrunk);
    mem::free(reuse);
    mem::free(e);

    ASSERT_TRUE(getTotalUsedMemory() == 0);

    TEST_PASS();
}
