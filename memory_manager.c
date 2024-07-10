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
    mem_list.slot_size = MEM_ALIGNMENT_BOUNDARY;
    mem_list.batch_count = 1;
    mem_list.bitmap_size = (MEM_BATCH_SLOT_COUNT + 7) / 8;
    mem_list.free_slots_bitmap = (unsigned char *)calloc(mem_list.bitmap_size, sizeof(unsigned char));
    memset(mem_list.free_slots_bitmap, 0xFF, mem_list.bitmap_size);
    mem_list.first_batch = (STRU_MEM_BATCH *)malloc(sizeof(STRU_MEM_BATCH));
    mem_list.first_batch->batch_mem = malloc(MEM_BATCH_SLOT_COUNT * MEM_ALIGNMENT_BOUNDARY);
    mem_list.first_batch->next_batch = NULL;
    mem_list.next_list = NULL;
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
void * mem_mngr_alloc(size_t size)
{
	if (size > MEM_ALIGNMENT_BOUNDARY) {
        return NULL; // Handle only requests <= MEM_ALIGNMENT_BOUNDARY
    }
    
    STRU_MEM_BATCH *batch = mem_list.first_batch;
    while (batch) {
        int pos = bitmap_find_first_bit(mem_list.free_slots_bitmap, mem_list.bitmap_size, 0);
        if (pos >= 0) {
            bitmap_set_bit(mem_list.free_slots_bitmap, mem_list.bitmap_size, pos);
            return (void *)((char *)batch->batch_mem + pos * MEM_ALIGNMENT_BOUNDARY);
        }
        if (!batch->next_batch) {
            batch->next_batch = (STRU_MEM_BATCH *)malloc(sizeof(STRU_MEM_BATCH));
            batch->next_batch->batch_mem = malloc(MEM_BATCH_SLOT_COUNT * MEM_ALIGNMENT_BOUNDARY);
            memset(mem_list.free_slots_bitmap, 0xFF, mem_list.bitmap_size);
            batch->next_batch->next_batch = NULL;
            mem_list.batch_count++;
        }
        batch = batch->next_batch;
    }
    return NULL;
}

/*
 * Free a chunk of memory pointed by ptr
 * Print out any error messages
 * @param: the pointer to the allocated memory slot
 */
void mem_mngr_free(void * ptr)
{
	STRU_MEM_BATCH *batch = mem_list.first_batch;
    while (batch) {
        if (ptr >= batch->batch_mem && ptr < (void *)((char *)batch->batch_mem + MEM_BATCH_SLOT_COUNT * MEM_ALIGNMENT_BOUNDARY)) {
            int pos = ((char *)ptr - (char *)batch->batch_mem) / MEM_ALIGNMENT_BOUNDARY;
            if (!bitmap_bit_is_set(mem_list.free_slots_bitmap, mem_list.bitmap_size, pos)) {
                printf("Error: Double freeing\n");
            } else {
                bitmap_clear_bit(mem_list.free_slots_bitmap, mem_list.bitmap_size, pos);
            }
            return;
        }
        batch = batch->next_batch;
    }
    printf("Error:\n");
}

