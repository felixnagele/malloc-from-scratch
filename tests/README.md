# Tests

This directory contains tests for the project, organized into subdirectories based on test type and purpose.
Tests use either 'TEST_PASS()' or assertions to report success or failure.

## Running Tests

Use the following commands in the root directory to run tests with CTest.

### All tests

```bash
cmake --build build && ctest --test-dir build
```

### All tests repeated N times

Replace `<N>` with the number of repetitions.

```bash
cmake --build build && ctest --test-dir build --repeat until-fail:<N> --output-on-failure
```

### Show all registered tests

```bash
cmake --build build && ctest --test-dir build --show-only
```

### Show failed tests

```bash
cmake --build build && ctest --test-dir build --output-on-failure
```

### Rerun failed tests

```bash
cmake --build build && ctest --test-dir build --rerun-failed
```

### Verbose output

```bash
cmake --build build && ctest --test-dir build --verbose
```

## Naming Convention

All test files follow: `test_<description>.cpp`

- Automatically discovered by CMake
- Each file produces one test executable
- Each executable is registered with the file name as the test name

## Memory Leak Detection with strace

The project includes automatic memory leak detection using `strace`. If strace is installed, CTest will automatically run additional tests that verify:

- **Program break restoration**: Checks if the first `brk()` address equals the last one
- **No memory leaks**: Ensures all allocated memory is properly freed
- **System call tracing**: Monitors `brk()` calls

### How It Works

1. CMake detects if `strace` is available
2. For each test, creates a `test_name_strace` variant
3. `tools/strace_test.sh` script:
   - Runs test under strace
   - Captures all `brk()` system calls
   - Compares first and last program break addresses
   - Reports memory leaks if addresses differ
