# Tests

This directory contains tests for the project, organized into subdirectories based on test type and purpose.

## Running Tests

Use the following commands in the root directory to run tests with CTest.

### All tests

```bash
cmake --build build && ctest --test-dir build
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
- Works recursively through subdirectories

## Directory Organization

### `unit/`

**Isolated function tests** - Each function tested independently

⚠️ **No strace tests** - Isolated tests don't free memory -> false positives

### `integration/`

**Multi-function workflows** - Functions working together

✅ **With strace tests** - Complete memory lifecycle (alloc + free)

### `edge_cases/`

**Boundary conditions and corner cases**

⚠️ **No strace tests** - Edge cases often don't include cleanup

### `stress/`

**High-load and performance scenarios**

✅ **With strace tests** - Stress tests clean up all allocations

### `concurrency/`

**Thread-safety and parallel execution**

✅ **With strace tests** - Multi-threaded tests include cleanup

### `correctness/`

**Data integrity and memory correctness**

✅ **With strace tests** - Correctness tests are mostly about leak detection

### `invalid/`

**Invalid operations and error handling**

⚠️ **No strace tests** - Invalid operations violate normal behavior

## Memory Leak Detection with strace

The project includes automatic memory leak detection using `strace`. If strace is installed, CTest will automatically run additional tests that verify:

- **Program break restoration**: Checks if `brk()` start address == end address
- **No memory leaks**: Ensures all allocated memory is properly freed
- **System call tracing**: Monitors `brk()`, `mmap()`, and `munmap()` calls

### How It Works

1. CMake detects if `strace` is available
2. For each test, creates a `test_name_strace` variant
3. `tools/strace_test.sh` script:
   - Runs test under strace
   - Captures all `brk()` system calls
   - Compares first and last program break addresses
   - Reports memory leaks if addresses differ
