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
        char* ptr = static_cast<char*>(mem::malloc(16));
        ASSERT_NOT_NULL(ptr);

        for (int i = 0; i < 32; i++)
        {
            ptr[i] = 'X';
        }

        mem::free(ptr);

        exit(1);
    }

    int status;
    waitpid(pid, &status, 0);

    ASSERT_TRUE(WIFEXITED(status));
    ASSERT_TRUE(WEXITSTATUS(status) == 255);

    TEST_PASS();
}
