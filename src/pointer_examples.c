/**
 * pointer_examples.c - Demonstration of C pointers with various tricky scenarios
 * 
 * This example demonstrates:
 * 1. Basic pointer usage and dereferencing
 * 2. Pointer arithmetic
 * 3. Function pointers
 * 4. Multiple levels of indirection (pointers to pointers)
 * 5. Array-pointer relationship
 * 6. Const pointers vs pointers to const
 * 7. Void pointers and type casting
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes for function pointer examples
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);

// Function that takes a function pointer as an argument
int calculate(int (*operation)(int, int), int a, int b);

void basic_pointer_examples() {
    printf("\n=== Basic Pointer Examples ===\n");
    
    int number = 42;
    int *ptr = &number;  // Pointer to number
    
    printf("Value of number: %d\n", number);
    printf("Address of number: %p\n", (void*)&number);
    printf("Value of ptr (address it points to): %p\n", (void*)ptr);
    printf("Value ptr points to (dereferencing): %d\n", *ptr);
    
    // Modifying value through pointer
    *ptr = 100;
    printf("After modification, number = %d\n", number);
    
    // Null pointer
    int *null_ptr = NULL;
    printf("Value of null_ptr: %p\n", (void*)null_ptr);
    
    // Danger: dereferencing a null pointer causes a segmentation fault
    printf("Dereferencing null_ptr would cause a crash if uncommented\n");
    // printf("Value at null_ptr: %d\n", *null_ptr);  // This would crash!
}

void pointer_arithmetic_examples() {
    printf("\n=== Pointer Arithmetic Examples ===\n");
    
    int numbers[] = {10, 20, 30, 40, 50};
    int *ptr = numbers;  // Same as &numbers[0]
    
    printf("Array address: %p\n", (void*)numbers);
    printf("First element address: %p\n", (void*)&numbers[0]);
    printf("First element via pointer: %d\n", *ptr);
    
    // Pointer arithmetic
    printf("Second element via pointer arithmetic: %d\n", *(ptr + 1));
    printf("Third element via pointer arithmetic: %d\n", *(ptr + 2));
    
    // Increment and decrement
    ptr++;  // Now points to the second element
    printf("After ptr++, *ptr = %d\n", *ptr);
    
    ptr += 2;  // Now points to the fourth element
    printf("After ptr += 2, *ptr = %d\n", *ptr);
    
    ptr--;  // Now points to the third element
    printf("After ptr--, *ptr = %d\n", *ptr);
    
    // Pointer differences
    int *end_ptr = &numbers[4];
    printf("Number of elements between pointers: %ld\n", end_ptr - ptr);
    
    // DANGER: Going beyond array bounds - undefined behavior!
    printf("Accessing beyond array bounds would be dangerous if uncommented\n");
    // printf("Beyond array bounds: %d\n", *(ptr + 10));  // Undefined behavior!
}

void pointer_to_pointer_examples() {
    printf("\n=== Pointer to Pointer Examples ===\n");
    
    int value = 42;
    int *ptr = &value;       // Pointer to value
    int **ptr_to_ptr = &ptr; // Pointer to pointer
    
    printf("value = %d\n", value);
    printf("*ptr = %d\n", *ptr);
    printf("**ptr_to_ptr = %d\n", **ptr_to_ptr);
    
    // Modifying the value through different levels
    **ptr_to_ptr = 100;
    printf("After **ptr_to_ptr = 100, value = %d\n", value);
    
    // Changing what the first pointer points to
    int another_value = 200;
    *ptr_to_ptr = &another_value;
    printf("After redirection, **ptr_to_ptr = %d\n", **ptr_to_ptr);
    printf("But value is still = %d\n", value);
    
    // Common use case: dynamically allocated 2D array
    int rows = 3, cols = 4;
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j;
        }
    }
    
    printf("Dynamic 2D array example:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
    
    // Cleanup dynamic memory
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void const_pointer_examples() {
    printf("\n=== Const Pointer Examples ===\n");
    
    int x = 10, y = 20;
    
    // 1. Non-const pointer to non-const value (regular pointer)
    int *regular_ptr = &x;
    *regular_ptr = 30;        // Can modify the value
    regular_ptr = &y;         // Can modify the pointer
    printf("After modifications: x = %d, y = %d\n", x, y);
    
    // 2. Const pointer to non-const value (pointer value is fixed)
    int * const fixed_ptr = &x;
    *fixed_ptr = 40;          // Can modify the value
    // fixed_ptr = &y;        // ERROR: Cannot modify the pointer
    printf("After fixed_ptr modification: x = %d\n", x);
    
    // 3. Non-const pointer to const value (value is fixed)
    const int *value_fixed_ptr = &x;
    // *value_fixed_ptr = 50; // ERROR: Cannot modify the value
    value_fixed_ptr = &y;     // Can modify the pointer
    printf("value_fixed_ptr now points to y = %d\n", *value_fixed_ptr);
    
    // 4. Const pointer to const value (both fixed)
    const int * const both_fixed_ptr = &x;
    // *both_fixed_ptr = 60;  // ERROR: Cannot modify the value
    // both_fixed_ptr = &y;   // ERROR: Cannot modify the pointer
    printf("both_fixed_ptr points to x = %d\n", *both_fixed_ptr);
}

void void_pointer_examples() {
    printf("\n=== Void Pointer Examples ===\n");
    
    int i = 42;
    float f = 3.14;
    char c = 'A';
    
    // Void pointer can point to any data type
    void *void_ptr;
    
    void_ptr = &i;
    printf("Void pointer pointing to int: %d\n", *(int*)void_ptr);
    
    void_ptr = &f;
    printf("Void pointer pointing to float: %.2f\n", *(float*)void_ptr);
    
    void_ptr = &c;
    printf("Void pointer pointing to char: %c\n", *(char*)void_ptr);
    
    // Memory allocation example using void pointer
    void_ptr = malloc(sizeof(int) * 5);
    int *int_array = (int*)void_ptr;
    for (int i = 0; i < 5; i++) {
        int_array[i] = i * 10;
    }
    
    printf("Dynamic array via void pointer: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", int_array[i]);
    }
    printf("\n");
    
    free(void_ptr);
}

void array_pointer_relationship() {
    printf("\n=== Array and Pointer Relationship ===\n");
    
    int numbers[5] = {10, 20, 30, 40, 50};
    int *ptr = numbers;  // Array name decays to pointer to first element
    
    printf("Array access vs pointer access:\n");
    for (int i = 0; i < 5; i++) {
        printf("numbers[%d] = %d, *(ptr + %d) = %d, ptr[%d] = %d\n",
               i, numbers[i], i, *(ptr + i), i, ptr[i]);
    }
    
    // Character arrays and strings
    char str[] = "Hello";
    char *str_ptr = str;
    
    printf("\nString as array: %s\n", str);
    printf("String as pointer: %s\n", str_ptr);
    
    // Difference: str is an array that cannot be reassigned
    // str = "World"; // ERROR: array type is not assignable
    
    // But str_ptr is a pointer that can be reassigned
    str_ptr = "World"; // This works because str_ptr now points to a string literal
    printf("After reassignment, str_ptr = %s\n", str_ptr);
    printf("Original str array = %s\n", str);
    
    // String literal is stored in read-only memory
    // str_ptr[0] = 'J'; // This might cause a segmentation fault
    
    // But elements of the array can be modified
    str[0] = 'J';
    printf("After modification, str = %s\n", str);
}

// Function pointer example implementations
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }

int calculate(int (*operation)(int, int), int a, int b) {
    return operation(a, b);
}

void function_pointer_examples() {
    printf("\n=== Function Pointer Examples ===\n");
    
    // Declare function pointer
    int (*op_ptr)(int, int);
    
    // Assign different functions to the pointer
    op_ptr = add;
    printf("Result of add via function pointer: %d\n", op_ptr(10, 5));
    
    op_ptr = subtract;
    printf("Result of subtract via function pointer: %d\n", op_ptr(10, 5));
    
    // Using the calculate function that takes a function pointer
    printf("Add via calculate: %d\n", calculate(add, 10, 5));
    printf("Multiply via calculate: %d\n", calculate(multiply, 10, 5));
    
    // Array of function pointers
    int (*operations[4])(int, int) = {add, subtract, multiply, divide};
    char *op_names[4] = {"Add", "Subtract", "Multiply", "Divide"};
    
    printf("\nUsing array of function pointers:\n");
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", op_names[i], operations[i](10, 5));
    }
}

int main() {
    printf("===== C POINTER EXAMPLES =====\n");
    
    basic_pointer_examples();
    pointer_arithmetic_examples();
    pointer_to_pointer_examples();
    const_pointer_examples();
    void_pointer_examples();
    array_pointer_relationship();
    function_pointer_examples();
    
    return 0;
}
