/*
 * CS 551 Project "Memory manager".
 * You do not need to turn in this file.	
 */

#include <stdio.h>
#include <string.h>
#include "memory_manager.h"
#include "interposition.h"

int main() {
    mem_mngr_init();

    printf("Testing memory allocation...\n");
    void *ptr1 = mem_mngr_alloc(5);
    printf("Allocated 5 bytes at %p\n", ptr1);

    void *ptr2 = mem_mngr_alloc(6);
    printf("Allocated 6 bytes at %p\n", ptr2);

    void *ptr3 = mem_mngr_alloc(7);
    printf("Allocated 7 bytes at %p\n", ptr3);

    void *ptr4 = mem_mngr_alloc(8);
    printf("Allocated 8 bytes at %p\n", ptr4);

    void *ptr5 = mem_mngr_alloc(9);
    printf("Allocated 9 bytes at %p\n", ptr5);

    mem_mngr_print_snapshot();
    printf("==============================================\n");

    printf("Testing memory free...\n");
    mem_mngr_free(ptr1);
    printf("Freed memory at %p\n", ptr1);


    mem_mngr_free(ptr2);
    printf("Freed memory at %p\n", ptr2);

    mem_mngr_free(ptr3);
    printf("Freed memory at %p\n", ptr3);

    mem_mngr_free(ptr4);
    printf("Freed memory at %p\n", ptr4);

    mem_mngr_free(ptr5);
    printf("Freed memory at %p\n", ptr5);

    mem_mngr_free(ptr1); // Double free test
    printf("Attempted to free memory at %p again (should report error)\n", ptr1);

    mem_mngr_print_snapshot();
    printf("==============================================\n");

    return 0;
}

