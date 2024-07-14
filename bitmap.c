/*
 * CS 551 Project "Memory manager".
 * This file needs to be turned in.	
 */

#include "common.h"
#include <stdio.h>

/*
 * Find the position (starting from 0) of the first bit that has value "val" in the "bitmap". 
 * @param bitmap: start address of the bitmap
 * @param size: size in bytes of the bitmap
 * @param val: 0 or non-0
 * @return: the position in the bitmap of the first 0 bit (if "val" is zero) or the first 1 bit (if "val" is non-zero)
 *          BITMAP_OP_NOT_FOUND (defined in "common.h") if the bit of interest does not exist in the bitmap
 *          BITMAP_OP_ERROR (defined in "common.h") on any other errors
 */
int bitmap_find_first_bit(unsigned char *bitmap, int size, int bit_value) {
    for (int byteIndex = 0; byteIndex < size; byteIndex++) {
        for (int bitIndex = 0; bitIndex < 8; bitIndex++) {
            int bit = (bitmap[byteIndex] >> bitIndex) & 1;
            if (bit == bit_value) {
                return byteIndex * 8 + bitIndex;
            }
        }
    }
    return -1; // No bit found
}
/*
 * Change the value of the "target_pos"-th bit (starting from 0) in the "bitmap" to 1.
 * @param bitmap: start address of the bitmap
 * @param size: size in bytes of the bit map
 * @param target_pos: the position of the bit of interest
 * @return: BITMAP_OP_SUCCEED (defined in "common.h") on success
 *          BITMAP_OP_ERROR (defined in "common.h") on any errors
 */
int bitmap_set_bit(unsigned char * bitmap, int size, int target_pos)
{
	int byteIndex = target_pos / 8;
    int bitIndex = target_pos % 8;
    if (byteIndex < size) {
        bitmap[byteIndex] |= (1 << bitIndex);
        return 0; // Success
    }
    return -1; // Fail 
}

/*
 * Change the value of the "target_pos"-th bit (starting from 0) in the "bitmap" to 0.
 * @param bitmap: start address of the bitmap
 * @param size: size in bytes of the bit map
 * @param target_pos: the position of the bit of interest
 * @return: BITMAP_OP_SUCCEED (defined in "common.h") on success
 *          BITMAP_OP_ERROR (defined in "common.h") on any errors
 */
int bitmap_clear_bit(unsigned char *bitmap, int size, int target_pos) 
{
    int byteIndex = target_pos / 8;
    int bitIndex = target_pos % 8;
    if (byteIndex < size) {
        bitmap[byteIndex] &= ~(1 << bitIndex);
        return 0; // Success
    }
    return -1; // Failure
}


/*
 * Test if the value of the "pos"-th bit (starting from 0) in the "bitmap" is 1.
 * @param bitmap: start address of the bitmap
 * @param size: size in bytes of the bit map
 * @param pos: the position of the bit of interest
 * @return: the value of the bit of interest (i.e., 0 or 1) on success
 *          BITMAP_OP_ERROR (defined in "common.h") on any errors
 */
int bitmap_bit_is_set(unsigned char *bitmap, int size, int target_pos) 
{
    int byteIndex = target_pos / 8;
    int bitIndex = target_pos % 8;
    if (byteIndex < size) {
        return (bitmap[byteIndex] & (1 << bitIndex)) != 0;
    }
    return 0;
}
/*
 * Print the content of a bitmap in starting from the first bit, and insert a space every 4 bits
 * @param bitmap: start address of the bitmap
 * @param size: size in bytes of the bit map
 * @return: BITMAP_OP_SUCCEED (defined in "common.h") on success
 *          BITMAP_OP_ERROR (defined in "common.h") on any errors
 * Do not change the implementation of this function.
 */
int bitmap_print_bitmap(unsigned char * bitmap, int size)
{
    int pos = 0;
    int total_bits = size * BIT_PER_BYTE;
    unsigned char current_byte = 0;

    if (NULL == bitmap)
    {
        printf("ERROR: NULL bit map!\n");
        return BITMAP_OP_ERROR;
    }

    printf("bitmap %p size %d bytes: ", bitmap, size);

    while (pos < total_bits)
    {
        int v = 0;
        
        current_byte = *(bitmap + pos/BIT_PER_BYTE);
        v = ((current_byte >> pos % BIT_PER_BYTE) & 0x01);
        printf("%d", v);
        if ((pos + 1) % 4 == 0)
        {
            printf("  ");
        }
        
        pos++;
    }

    printf("\n");

    return BITMAP_OP_SUCCEED;
}

int bitmap_check_bit(unsigned char *bitmap, int size, int target_pos) {
    int byteIndex = target_pos / 8;
    int bitIndex = target_pos % 8;
    if (byteIndex < size) {
        return (bitmap[byteIndex] >> bitIndex) & 1;
    }
    return -1; // Error: out of bounds
}

int bitmap_find_free_slot(unsigned char *bitmap, int size) {
    for (int byteIndex = 0; byteIndex < size; byteIndex++) {
        for (int bitIndex = 0; bitIndex < 8; bitIndex++) {
            int bitPosition = byteIndex * 8 + bitIndex;
            if ((bitmap[byteIndex] & (1 << bitIndex)) == 0) {
                // Bit is free (0 means free in our bitmap convention)
                return bitPosition;
            }
        }
    }
    // No free slot found
    return -1;
}

