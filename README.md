# malloc-from-scratch 🧩

A custom memory allocator implementation in C++. This project implements malloc, free, calloc, and realloc from scratch using only native C++ (no external dependencies). This is done by using the system call `sbrk()` to manage memory at a low level.

![CI](https://github.com/felixnagele/malloc-from-scratch/workflows/CI/badge.svg)

## ✨ Features

- Implements `malloc`, `free`, `calloc`, and `realloc` from scratch
- Uses native C++ only (no external dependencies)
- Custom memory management strategies
- Test suite with different test types and memory leak detection

## 🛠 Requirements

- CMake 3.14 or higher
- C++17 compatible compiler
- **No external libraries required** - 100% native C++

## 🚀 Quick Start - Linux (Ubuntu/Debian)

### Step 1: Clone the repo

```bash
git clone https://github.com/felixnagele/malloc-from-scratch.git
cd malloc-from-scratch
```

### Step 2: Update and install build tools and dependencies

```bash
sudo apt update && sudo sudo apt install -y build-essential cmake clang-format clang-tidy strace
```

### Step 3: Configure build directory

```bash
cmake -S . -B build
```

### Step 4: Build the project

```bash
cmake --build build
```

### One-liner setup

```bash
git clone https://github.com/felixnagele/malloc-from-scratch.git && cd malloc-from-scratch && sudo apt update && sudo apt install -y build-essential cmake clang-format clang-tidy strace && cmake -S . -B build && cmake --build build
```

### Optional: VS Code setup (clang-format)

#### Step 1: Install the clang-format extension

Install the [Clang-Format extension](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format) from the VS Code marketplace.

#### Step 2: Configure VS Code settings

VS Code usually detects `clang-format` automatically if it is installed on your system. If VS Code cannot find the formatter or formatting does not work, you can manually specify the path to the `clang-format` executable.

Open your VS Code user settings (`settings.json`):

`Ctrl + Shift + P -> "Preferences: Open Settings (JSON)"`

Add the following entry, replacing the path with the correct one for your system:

```json
{
  "clang-format.executable": "/path/to/clang-format"
}
```

Example for Ubuntu/Debian:

```json
{
  "clang-format.executable": "/usr/bin/clang-format"
}
```

## 📖 Usage - Linux (Ubuntu/Debian)

### Verify installation

```bash
gcc --version
cmake --version
clang-format --version
clang-tidy --version
strace --version
```

### Formatting

#### Check formatting (what CI does)

```bash
find src include tests -name "*.cpp" -o -name "*.h" | xargs clang-format --dry-run -Werror
```

#### Auto-fix formatting

```bash
find src include tests -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

### Linting

#### Run clang-tidy static analysis (what CI does)

```bash
find src tests -name "*.cpp" -exec clang-tidy -p build {} \;
```

## 🧪 Running Tests

Run tests locally:

```bash
cmake --build build && ctest --test-dir build
```

If you use VS Code, you can also run the tests locally via the task defined in `.vscode/tasks.json`. Use `Ctrl + Shift + B` to run the build and tests task.

For detailed test usage with instructions, see [tests/README.md](./tests/README.md).

Tests also run automatically in CI/pipeline [Actions](https://github.com/felixnagele/malloc-from-scratch/actions).

## Contributing, License & Support

See [Community Profile](https://github.com/felixnagele/malloc-from-scratch/community) for guidelines, license, and support.
