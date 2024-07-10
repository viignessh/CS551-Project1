/*
 * CS 551 Project "Memory manager".
 * You do not need to turn in this file.	
 */

#include <stdio.h>
#include <string.h>
#include "memory_manager.h"
#include "interposition.h"

void test_memory_allocation() {
    printf("Testing memory allocation...\n");

    void *ptr1 = mem_mngr_alloc(5);
    printf("Allocated 5 bytes at %p\n", ptr1);

    void *ptr2 = mem_mngr_alloc(6);
    printf("Allocated 6 bytes at %p\n", ptr2);

    void *ptr3 = mem_mngr_alloc(7);
    printf("Allocated 7 bytes at %p\n", ptr3);

    void *ptr4 = mem_mngr_alloc(8);
    printf("Allocated 8 bytes at %p\n", ptr4);

    void *ptr5 = mem_mngr_alloc(9); // Should return NULL
    if (ptr5 == NULL) {
        printf("Allocation of 9 bytes failed as expected (NULL returned)\n");
    } else {
        printf("Allocated 9 bytes at %p (unexpected)\n", ptr5);
    }

    mem_mngr_print_snapshot();
}

void test_memory_free() {
    printf("\nTesting memory free...\n");

    void *ptr1 = mem_mngr_alloc(5);
    void *ptr2 = mem_mngr_alloc(6);

    mem_mngr_free(ptr1);
    printf("Freed memory at %p\n", ptr1);

    mem_mngr_free(ptr2);
    printf("Freed memory at %p\n", ptr2);

    // Attempt to free the same memory again (should report error)
    mem_mngr_free(ptr1);
    printf("Attempted to free memory at %p again (should report error)\n", ptr1);

    mem_mngr_print_snapshot();
}

int main(void) {
    mem_mngr_init();

    // Test memory allocation
    test_memory_allocation();

    // Test memory free
    test_memory_free();

    mem_mngr_leave();

    return 0;
}