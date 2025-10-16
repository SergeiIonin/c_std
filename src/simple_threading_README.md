# Custom Lock Implementation in C

This example demonstrates a simple but effective custom lock implementation in C using atomic operations and a two-level locking mechanism with flag, guard, and queue.

## Overview

The implementation uses three key components:

```c
typedef struct {
    volatile bool flag;    // Main lock flag
    volatile bool guard;   // Guard to protect the flag
    int queue;             // Queue counter for waiting threads
} simple_lock_t;
```

## Understanding the Lock Design

### Why Two Boolean Variables?

The implementation uses two boolean variables (`flag` and `guard`) as part of a two-level locking strategy:

1. **The Flag**: Represents the actual lock. When a thread holds the lock, `flag` is set to `true`.

2. **The Guard**: Protects the critical section where the `flag` is checked and potentially modified. The `guard` ensures that only one thread at a time can attempt to acquire the lock, preventing race conditions during lock acquisition.

This two-level approach is known as a "Test and Test-and-Set" (TTAS) lock, which is more efficient than a simple "Test-and-Set" (TAS) lock in high-contention scenarios.

### The Queue

The `queue` counter tracks how many threads are waiting to acquire the lock. This can be used to implement more sophisticated scheduling policies or for lock statistics.

## Lock Acquisition Explained

The `simple_lock_acquire` function demonstrates the lock acquisition strategy:

```c
void simple_lock_acquire(simple_lock_t *lock) {
    // Increment queue to indicate intention to acquire lock
    __atomic_fetch_add(&lock->queue, 1, __ATOMIC_SEQ_CST);
    
    // Try to acquire the lock
    while (true) {
        // First, acquire the guard using atomic test-and-set
        while (__atomic_test_and_set(&lock->guard, __ATOMIC_ACQUIRE)) {
            // Spin waiting for the guard
            usleep(10);
        }
        
        // Check if the main lock is available
        if (!lock->flag) {
            // Lock is free, take it
            lock->flag = true;
            lock->guard = false; // Release the guard
            break; // Lock acquired
        }
        
        // Lock is not available, release the guard and try again
        lock->guard = false;
        usleep(100); // Short sleep to reduce CPU usage
    }
    
    // Decrement queue as this thread now has the lock
    __atomic_fetch_sub(&lock->queue, 1, __ATOMIC_SEQ_CST);
}
```

### Key Components of Lock Acquisition

1. **Queue Management**
   - The function starts by atomically incrementing the queue counter
   - This indicates that a thread intends to acquire the lock

2. **The Guard Acquisition**
   - `__atomic_test_and_set(&lock->guard, __ATOMIC_ACQUIRE)` is used to atomically set the guard to `true` and return its previous value
   - If the previous value was `false`, the thread acquires the guard
   - If it was `true`, the thread spins until it can acquire the guard

3. **The Lock Check**
   - Once the guard is acquired, the thread checks if the main lock (`flag`) is available
   - If available, the thread takes the lock by setting `flag` to `true`
   - The thread then releases the guard and exits the loop

4. **Spin with Backoff**
   - If the lock is not available, the thread releases the guard
   - It then waits for a short time (`usleep(100)`) to reduce CPU usage before trying again
   - This backoff strategy reduces contention on the guard

## Understanding `__atomic_test_and_set`

The `__atomic_test_and_set` function is a GCC built-in atomic operation that:

1. Atomically sets a boolean value to `true`
2. Returns the previous value

This creates a critical atomic operation that allows only one thread to succeed when multiple threads attempt to set the value simultaneously.

### Memory Order Semantics

The `__ATOMIC_ACQUIRE` parameter specifies the memory ordering constraint:

- It ensures that all memory operations after the atomic operation do not move before it
- This creates a memory barrier that prevents certain kinds of reordering

## Why This Approach Works

The two-level locking with guard and flag is more efficient than a simple spinlock because:

1. **Reduced Bus Traffic**: Threads first check if the lock is available before attempting to acquire it
2. **Reduced Contention**: The guard protects the critical section where the lock is checked and acquired
3. **Fairness**: The queue counter could be extended to implement fair scheduling policies
4. **CPU Efficiency**: The backoff strategy reduces CPU usage during contention

## Comparison with Other Lock Implementations

| Lock Type | Advantages | Disadvantages |
|-----------|------------|---------------|
| Simple Spinlock | Simple implementation | High CPU usage, cache thrashing |
| Mutex (pthread) | Efficient thread sleeping | System call overhead |
| Our TTAS Lock | Reduced contention, tunable | More complex implementation |

## Performance Considerations

For high-performance applications, consider:

1. Tuning the backoff times (`usleep` durations)
2. Implementing exponential backoff instead of fixed delay
3. Using queue information for more sophisticated scheduling

## Further Reading

- [The Art of Multiprocessor Programming](https://dl.acm.org/doi/book/10.5555/2385452)
- [Linux Kernel Spinlock Implementation](https://github.com/torvalds/linux/blob/master/include/linux/spinlock.h)
- [C11 Atomic Operations](https://en.cppreference.com/w/c/atomic)
