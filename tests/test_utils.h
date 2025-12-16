#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <cstring>
#include <iostream>

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

#endif // TEST_UTILS_H
