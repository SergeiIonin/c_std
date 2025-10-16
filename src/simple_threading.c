/**
 * simple_threading.c - Demonstration of basic multithreading with a custom lock
 * 
 * This example shows:
 * 1. How to create and manage threads using POSIX threads (pthreads)
 * 2. Implementation of a simple lock mechanism with flag, guard, and queue
 * 3. Thread synchronization where 3 threads acquire the lock one after another
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For sleep function
#include <stdbool.h> // For boolean type

// Simple lock implementation
typedef struct {
    volatile bool flag;    // Main lock flag
    volatile bool guard;   // Guard to protect the flag
    int queue;             // Queue counter for waiting threads
} simple_lock_t;

// Initialize the lock
void simple_lock_init(simple_lock_t *lock) {
    lock->flag = false;    // Not locked
    lock->guard = false;   // Guard not in use
    lock->queue = 0;       // No waiting threads
}

// Lock acquisition with queue
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

// Release the lock
void simple_lock_release(simple_lock_t *lock) {
    // Simply release the lock flag (no need for guard here)
    __atomic_store_n(&lock->flag, false, __ATOMIC_RELEASE);
}

// Global lock
simple_lock_t lock;

// Thread function
void* thread_function(void *arg) {
    int thread_id = *((int*)arg);
    
    // Try to acquire the lock
    simple_lock_acquire(&lock);
    
    // Critical section
    printf("Hello from thread #%d\n", thread_id);
    
    // Sleep to simulate some work
    sleep(1);
    
    printf("Thread #%d releasing the lock\n", thread_id);
    
    // Release the lock
    simple_lock_release(&lock);
    
    return NULL;
}

int main() {
    pthread_t threads[3];
    int thread_ids[3] = {1, 2, 3};
    
    // Initialize the lock
    simple_lock_init(&lock);
    
    printf("Starting threads...\n");
    
    // Create threads
    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
        
        // Small delay between thread creations
        usleep(10000);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("All threads have completed\n");
    
    return 0;
}
