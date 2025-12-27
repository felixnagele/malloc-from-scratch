#include "malloc_from_scratch/memory_allocator.h"
#include "test_utils.h"
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();
    ASSERT_TRUE(pid != -1);

    if (pid == 0)
    {
        char* ptr = static_cast<char*>(mem::malloc(64));
        ASSERT_NOT_NULL(ptr);

        char* interior_ptr = ptr + 16;
        // NOLINTNEXTLINE - Intentionally testing interior pointer
        mem::free(interior_ptr);

        exit(1);
    }

    int status;
    waitpid(pid, &status, 0);

    ASSERT_TRUE(WIFEXITED(status));
    ASSERT_TRUE(WEXITSTATUS(status) == 255);

    TEST_PASS();
}
