#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "malloc_from_scratch/memory_internal.h"
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

inline void assert_true(bool condition, const char* condition_str, const char* file, int line)
{
    if (!condition)
    {
        std::cerr << "FAILED: " << file << ":" << line << " - Assertion '" << condition_str
                  << "' failed" << std::endl;
        exit(1);
    }
}

inline void assert_false(bool condition, const char* condition_str, const char* file, int line)
{
    if (condition)
    {
        std::cerr << "FAILED: " << file << ":" << line << " - Assertion 'not " << condition_str
                  << "' failed" << std::endl;
        exit(1);
    }
}

inline void assert_null(void* ptr, const char* file, int line)
{
    if (ptr != nullptr)
    {
        std::cerr << "FAILED: " << file << ":" << line << " - Expected nullptr, got " << ptr
                  << std::endl;
        exit(1);
    }
}

inline void assert_not_null(void* ptr, const char* file, int line)
{
    if (ptr == nullptr)
    {
        std::cerr << "FAILED: " << file << ":" << line << " - Expected non-null pointer"
                  << std::endl;
        exit(1);
    }
}

inline void test_pass()
{
    std::cout << "PASSED" << std::endl;
    exit(0);
}

#define ASSERT_TRUE(condition) assert_true((condition), #condition, __FILE__, __LINE__)
#define ASSERT_FALSE(condition) assert_false((condition), #condition, __FILE__, __LINE__)
#define ASSERT_NULL(ptr) assert_null((ptr), __FILE__, __LINE__)
#define ASSERT_NOT_NULL(ptr) assert_not_null((ptr), __FILE__, __LINE__)
#define TEST_PASS() test_pass()

// Fills `size` bytes of `ptr` with a constant value
inline void fill_bytes(void* ptr, size_t size, unsigned char value)
{
    unsigned char* data = static_cast<unsigned char*>(ptr);
    for (size_t i = 0; i < size; i++)
    {
        data[i] = value;
    }
}

// Asserts every byte of `ptr` equals `value`
inline void check_bytes(void* ptr, size_t size, unsigned char value)
{
    unsigned char* data = static_cast<unsigned char*>(ptr);
    for (size_t i = 0; i < size; i++)
    {
        ASSERT_TRUE(data[i] == value);
    }
}

// Runs `body` in a forked child process and asserts the child exits with `expected_code`.
// run_in_child(0, ...)   -> the child exited normally
// run_in_child(255, ...) -> the child was killed by an allocator abort
void run_in_child(int expected_code, void (*body)())
{
    pid_t pid = fork();
    ASSERT_TRUE(pid != -1);

    if (pid == 0)
    {
        body();
        exit(0); // only reached if body ran to the end without exiting
    }

    int status;
    waitpid(pid, &status, 0);
    ASSERT_TRUE(WIFEXITED(status));
    ASSERT_TRUE(WEXITSTATUS(status) == expected_code);
}

// Internal test helpers for convenience
using mem::internal::getFreeBlockInfo;
using mem::internal::getTotalUsedMemory;

#endif // TEST_UTILS_H
