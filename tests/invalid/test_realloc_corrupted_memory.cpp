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

        for (int i = 0; i < 64; i++)
        {
            ptr[i] = static_cast<char>(i);
        }

        for (int i = 64; i < 128; i++)
        {
            ptr[i] = 'Z';
        }

        mem::realloc(ptr, 128);

        exit(1);
    }

    int status;
    waitpid(pid, &status, 0);

    ASSERT_TRUE(WIFEXITED(status));
    ASSERT_TRUE(WEXITSTATUS(status) == 255);

    TEST_PASS();
}
