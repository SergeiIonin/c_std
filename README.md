# Learning C Programming By Examples

This repository contains a collection of C programming examples for learning purposes. Each example demonstrates specific C concepts and is well-documented for easy understanding.

## Getting Started

### Prerequisites
- GCC compiler or any C compiler
- Make utility (for using the Makefile)

### Compiling and Running Examples

#### Using Make
The repository includes a Makefile to simplify compilation:

```bash
# Compile all examples
make all

# Compile a specific example (e.g., hello_world)
make hello_world

# Run a specific example
make run_hello_world

# Clean compiled files
make clean
```

#### Manual Compilation
If you prefer to compile manually, you can use GCC directly:

```bash
# Navigate to the source directory
cd src

# Compile a specific file
gcc hello_world.c -o hello_world

# Run the compiled program
./hello_world
```

## Examples

### 1. Hello World (`src/hello_world.c`)
A simple program that outputs "Hello, World!" to the console. This example demonstrates:
- Basic program structure
- Including header files
- Using the `printf` function
- The `main` function as the entry point

### 2. Simple Threading with Custom Lock (`src/simple_threading.c`)
A multithreading example that demonstrates:
- Creating and managing POSIX threads (pthreads)
- Implementation of a simple lock mechanism with flag, guard, and queue
- Thread synchronization with three threads acquiring a lock sequentially
- Atomic operations for thread safety

### 3. Pointer Examples (`src/pointer_examples.c`)
A comprehensive guide to C pointers covering:
- Basic pointer operations and dereferencing
- Pointer arithmetic and array relationships
- Function pointers and callbacks
- Multiple levels of indirection (pointers to pointers)
- Const pointers and pointers to const data
- Void pointers and type casting
- Common pitfalls and best practices

## License

This project is provided for educational purposes only.
