/*
 * CS 551 Project "Memory manager".
 * This file needs to be turned in.	
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.c"
#include "memory_manager.h"

static STRU_MEM_LIST * mem_pool = NULL;
static STRU_MEM_LIST mem_list;


/*
 * Print out the current status of the memory manager.
 * Reading this function may help you understand how the memory manager organizes the memory.
 * Do not change the implementation of this function. It will be used to help the grading.
 */
void mem_mngr_print_snapshot(void)
{
    STRU_MEM_LIST * mem_list = NULL;

    printf("============== Memory snapshot ===============\n");

    mem_list = mem_pool; // Get the first memory list
    while(NULL != mem_list)
    {
        STRU_MEM_BATCH * mem_batch = mem_list->first_batch; // Get the first mem batch from the list 

        printf("mem_list %p slot_size %d batch_count %d free_slot_bitmap %p\n", 
                   mem_list, mem_list->slot_size, mem_list->batch_count, mem_list->free_slots_bitmap);
        bitmap_print_bitmap(mem_list->free_slots_bitmap, mem_list->bitmap_size);

        while (NULL != mem_batch)
        {
            printf("\t mem_batch %p batch_mem %p\n", mem_batch, mem_batch->batch_mem);
            mem_batch = mem_batch->next_batch; // get next mem batch
        }

        mem_list = mem_list->next_list;
    }

    printf("==============================================\n");
}

/*
 * Initialize the memory manager with 8 bytes slot size mem_list.
 * Initialize this list with 1 batch of slots.
 */
void mem_mngr_init(void)
{
	printf("Initializing memory manager...\n");
    	mem_list.slot_size = MEM_ALIGNMENT_BOUNDARY;
    	mem_list.batch_count = 1;
    	mem_list.bitmap_size = (MEM_BATCH_SLOT_COUNT + 7) / 8;
    	mem_list.free_slots_bitmap = (unsigned char *)calloc(mem_list.bitmap_size, sizeof(unsigned char));
    	memset(mem_list.free_slots_bitmap, 0xFF, mem_list.bitmap_size); // Set all bits to 1 (all slots free)
    	mem_list.first_batch = (STRU_MEM_BATCH *)malloc(sizeof(STRU_MEM_BATCH));
    	mem_list.first_batch->batch_mem = malloc(MEM_BATCH_SLOT_COUNT * MEM_ALIGNMENT_BOUNDARY);
    	mem_list.first_batch->next_batch = NULL;
    	mem_list.next_list = NULL;
	printf("Memory manager initialized successfully.\n");
}

/*
 * Clean up the memory manager (e.g., release all the memory allocated)
 */
void mem_mngr_leave(void)
{
	STRU_MEM_BATCH *batch = mem_list.first_batch;
    while (batch) {
        STRU_MEM_BATCH *next_batch = batch->next_batch;
        free(batch->batch_mem);
        free(batch);
        batch = next_batch;
    }
    free(mem_list.free_slots_bitmap);
    mem_list.first_batch = NULL;
}

/*
 * Allocate a chunk of memory 	
 * @param size: size in bytes to be allocated
 * @return: the pointer to the allocated memory slot
 */
void *mem_mngr_alloc() {
    int aligned_size = MEM_ALIGNMENT_BOUNDARY;
    int batch_slot_count = MEM_BATCH_SLOT_COUNT;
    int bitmap_size = (batch_slot_count + 7) / 8;

    STRU_MEM_BATCH *batch = mem_list.first_batch;
    while (batch) {
        int pos = bitmap_find_free_slot(mem_list.free_slots_bitmap, mem_list.bitmap_size);
        if (pos != -1)
       	{
            void *ptr = (char *)batch->batch_mem + pos * aligned_size;
            bitmap_set_bit(mem_list.free_slots_bitmap, bitmap_size, pos);
            return ptr;
        }
        batch = batch->next_batch;
    }

    // No free slots found, allocate a new batch
    STRU_MEM_BATCH *new_batch = (STRU_MEM_BATCH *)malloc(sizeof(STRU_MEM_BATCH));
    if (!new_batch) {
        printf("Error: Failed to allocate new batch\n");
        return NULL;
    }

    new_batch->batch_mem = malloc(batch_slot_count * aligned_size);
    if (!new_batch->batch_mem) {
        printf("Error: Failed to allocate batch memory\n");
        free(new_batch);
        return NULL;
    }

    // Initialize the bitmap for the new batch
    unsigned char *new_bitmap = (unsigned char *)calloc(bitmap_size, sizeof(unsigned char));
    if (!new_bitmap) {
        printf("Error: Failed to allocate bitmap for new batch\n");
        free(new_batch->batch_mem);
        free(new_batch);
        return NULL;
    }
    memset(new_bitmap, 0xFF, bitmap_size);

    new_batch->next_batch = mem_list.first_batch;
    mem_list.first_batch = new_batch;
    mem_list.free_slots_bitmap = new_bitmap;

    // Return the first slot of the newly allocated batch
    void *ptr = new_batch->batch_mem;
    bitmap_set_bit(mem_list.free_slots_bitmap, bitmap_size, 0); // Mark first slot as allocated
    return ptr;
}
 


/*
 * Free a chunk of memory pointed by ptr
 * Print out any error messages
 * @param: the pointer to the allocated memory slot
 */
void mem_mngr_free(void *ptr) {
    if (!ptr) {
        printf("Error: Pointer not found in any batch\n");
        return;
    }

    STRU_MEM_BATCH *batch = mem_list.first_batch;
    while (batch) {
        if (ptr >= batch->batch_mem && ptr < (void *)((char *)batch->batch_mem + MEM_BATCH_SLOT_COUNT * MEM_ALIGNMENT_BOUNDARY)) {
            int pos = ((char *)ptr - (char *)batch->batch_mem) / MEM_ALIGNMENT_BOUNDARY;
            if (bitmap_check_bit(mem_list.free_slots_bitmap, mem_list.bitmap_size, pos) == 0) {
                printf("Error: Double freeing\n");
                return;
            }
            bitmap_set_bit(mem_list.free_slots_bitmap, mem_list.bitmap_size, pos); // Mark as free
            return;
        }
        batch = batch->next_batch;
    }

    printf("Error: Pointer not found in any batch\n");
}

