////////////////////////////////////////////////////////////////////////////////
// Main File:        mem.c
// This File:        mem.c
// Other Files:      mem.h
// Semester:         CS 354 Fall 2018
//
// Author:           Matt Bachmeier
// Email:            mbachmeier@wisc.edu
// CS Login:         bachmeier
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          N/A
//                   
//
// Online sources:   N/A
//                    
//                   
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "mem.h"

/*
 * This structure serves as the header for each allocated and free block
 * It also serves as the footer for each free block
 * The blocks are ordered in the increasing order of addresses 
 */
typedef struct blk_hdr {                         
        int size_status;
  
    /*
    * Size of the block is always a multiple of 8
    * => last two bits are always zero - can be used to store other information
    *
    * LSB -> Least Significant Bit (Last Bit)
    * SLB -> Second Last Bit 
    * LSB = 0 => free block
    * LSB = 1 => allocated/busy block
    * SLB = 0 => previous block is free
    * SLB = 1 => previous block is allocated/busy
    * 
    * When used as the footer the last two bits should be zero
    */

    /*
    * Examples:
    * 
    * For a busy block with a payload of 20 bytes (i.e. 20 bytes data + an additional 4 bytes for header)
    * Header:
    * If the previous block is allocated, size_status should be set to 27
    * If the previous block is free, size_status should be set to 25
    * 
    * For a free block of size 24 bytes (including 4 bytes for header + 4 bytes for footer)
    * Header:
    * If the previous block is allocated, size_status should be set to 26
    * If the previous block is free, size_status should be set to 24
    * Footer:
    * size_status should be 24
    * 
    */
} blk_hdr;

/* Global variable - This will always point to the first block
 * i.e. the block with the lowest address */
blk_hdr *first_blk = NULL;

/*
 * Note: 
 *  The end of the available memory can be determined using end_mark
 *  The size_status of end_mark has a value of 1
 *
 */

/* 
 * Function for allocating 'size' bytes
 * Returns address of allocated block on success 
 * Returns NULL on failure 
 * Here is what this function should accomplish 
 * - Check for sanity of size - Return NULL when appropriate 
 * - Round up size to a multiple of 8 
 * - Traverse the list of blocks and allocate the best free block which can accommodate the requested size 
 * - Also, when allocating a block - split it into two blocks
 * Tips: Be careful with pointer arithmetic 
 */                    
void* Alloc_Mem(int size) {
    // Make sure the size they want is valid
    if (size < 1) {
            //printf("Size specified to allocate must be greater than 0\n");
            return NULL;
    }

    // total block size. Must include the header as well as the size
    int totalBlockSize = (size+sizeof(blk_hdr));

    // size of a double word, used for aligning values
    int doubleWordSize = 8;

    // Make sure size is double word aligned
    int remainder;  // used in if statement to add padding for alignment
    if (totalBlockSize < doubleWordSize) {
            // if block size is small just make it a double word
            totalBlockSize = doubleWordSize; 
    } else if (totalBlockSize % doubleWordSize != 0) {
            remainder = totalBlockSize % 8;
            // padd total block size to align with double word
            totalBlockSize = totalBlockSize + (doubleWordSize - remainder);
    }

    // Point to the start of heap
    blk_hdr *currentBlock = first_blk;

    // Keep track of the best fit block
    blk_hdr *bestFitBlock = NULL;

    // size of LSB for data info
    int leastSigBits = 0;

    // size of block w/o data info
    int sizeOfBlock = 0;

    // tracks best block size for comparison
    int bestBlockSize = 0;

    if (currentBlock == NULL) {
            //printf("Initialize memory for heap before allocating\n");
            return NULL;
    }

    // Use best fit placement and split the block if possible
    // Check all blocks in memory before choosing best fit
    while ((currentBlock->size_status) != 1) {
        // header data info    
        leastSigBits = (currentBlock->size_status) % doubleWordSize;

        // size of the block we are pointing to
        sizeOfBlock = (currentBlock->size_status) - leastSigBits;

        if ((leastSigBits % 2) != 0) {
                // the block is allocated. Move to next block.
                currentBlock = (blk_hdr*)((char*)currentBlock + sizeOfBlock);
        } else {
                if (sizeOfBlock == totalBlockSize) {
                        // Current block is perfect size, pick immediately
                        bestFitBlock = currentBlock;

                        if (leastSigBits == 2) {
                                // When previous block is allocated remember
                                // that. 2 + 1 = 3, previous allocate
                                // current allocated
                                bestFitBlock->size_status = totalBlockSize + 3;
                        } else {
                                // when previous block isn't allocated remember
                                bestFitBlock->size_status = totalBlockSize + 1;
                        }

                        // Update next block 
                        blk_hdr *nextBlock = (blk_hdr*)((char*)bestFitBlock +
                                        totalBlockSize);

                        // When next block not the end, update data
                        if (nextBlock->size_status != 1) {
                                nextBlock->size_status = (nextBlock->
                                           size_status)+2;
                        }

                        // return address of payload
                        return (blk_hdr*)((char*)bestFitBlock +
                                        sizeof(blk_hdr*));
                } else if (sizeOfBlock > totalBlockSize) {
                        // When there is no best block yet, get the size of
                        // the current large enough one
                        if (bestBlockSize == 0) {
                                bestFitBlock = currentBlock;
                                bestBlockSize = (bestFitBlock->size_status-
                                         ((bestFitBlock->size_status) %
                                         doubleWordSize));
                        } else if (sizeOfBlock < bestBlockSize) {
                        // When there is a best block, see if it is a better
                        // fit than before
                                bestFitBlock = currentBlock;
                                bestBlockSize = (bestFitBlock->size_status)-
                                ((bestFitBlock->size_status)%doubleWordSize);
                        }

                        // Move to the next block to check
                        currentBlock = (blk_hdr*)((char*)currentBlock +
                                sizeOfBlock);
                } else {
                        // current block was too small
                        // move to next block
                        currentBlock = (blk_hdr*)((char*)currentBlock +
                                sizeOfBlock);
                }
        }
    }

    // Not enough room for the allocation
    if (bestFitBlock == NULL) {
            //printf("Not enough memory available for allocation\n");
            return NULL;
    }

    // get the size of split to happen
    int sizeOfSplit = (bestBlockSize - totalBlockSize);

    // create the split and give it data
    blk_hdr *splitBlockData = (blk_hdr*)((char*)bestFitBlock + totalBlockSize);
    // sets size of split and data for previous block
    splitBlockData->size_status = (sizeOfSplit) + 2;

    // Location of footer of split block
    blk_hdr *splitBlockFoot = (blk_hdr*)((char*)splitBlockData +
                    sizeOfSplit - 4);

    // Give footer size of split block
    splitBlockFoot->size_status = sizeOfSplit;

    // See if next block can be coalated
    blk_hdr *nextBlock = (blk_hdr*)((char*)splitBlockData + sizeOfSplit);

    // Info on next block
    int nextBlockSizeData = nextBlock->size_status;


    if (nextBlockSizeData != 1 && nextBlockSizeData % 2 == 0) {
            int nextBlockSize = nextBlockSizeData - (nextBlockSizeData %
                    doubleWordSize);

            // set the size of the new coalated block
            splitBlockData->size_status = (splitBlockData->size_status) +
                    nextBlockSize;

            // add footer of coalated block
            splitBlockFoot = (blk_hdr*)((char*)splitBlockData + sizeOfSplit +
                             nextBlockSize - 4);

            // give size to the footer of coalated block
            splitBlockFoot->size_status = sizeOfSplit + nextBlockSize;
    }

    // check previous block data
    if (leastSigBits == 2) { 
            // if it had been allocated remember that in LSB
            bestFitBlock->size_status = totalBlockSize + 3;
    } else {  // if it hasnt been allocated remember that in LSB
            bestFitBlock->size_status = totalBlockSize + 1;
    } 

    // the address to return, right after the header/start of payload
    blk_hdr *blockReturn = (blk_hdr*)((char*)bestFitBlock + sizeof(blk_hdr*));

    return blockReturn;
}

/* 
 * Function for freeing up a previously allocated block 
 * Argument - ptr: Address of the block to be freed up 
 * Returns 0 on success 
 * Returns -1 on failure 
 * Here is what this function should accomplish 
 * - Return -1 if ptr is NULL
 * - Return -1 if ptr is not 8 byte aligned or if the block is already freed
 * - Mark the block as free 
 * - Coalesce if one or both of the immediate neighbours are free 
 */                    
int Free_Mem(void *ptr) {                        
    // Your code goes in here 
    if (ptr == NULL) {
            //printf("Memory is empty\n");
            return -1;
    }

    // used to align blocks
    int doubleWordSize = 8;

    // create header for block to be freed to change info
    blk_hdr *blockToFree = (blk_hdr*)((char*)ptr - 4);

    // Get data in header now
    int sizeData = blockToFree->size_status;

    // get least sig bits data
    int LSBData = sizeData % doubleWordSize;

    if (LSBData == 0) {
            //printf("Block is already free\n");
            return -1;
    }

    // size of the block to free
    int blockToFreeSize = sizeData - LSBData;

    // When block isn't double word aligned
    if (blockToFreeSize % doubleWordSize != 0) {
            //printf("Block to free doesn't use double word alignment\n");
            return -1;
    } else {
            // subtract one from size_status to indictate it's free
            blockToFree->size_status = (blockToFree->size_status) - 1;

            // create the footer
            blk_hdr *blockToFreeFoot = (blk_hdr*)((char*)blockToFree +
                            blockToFreeSize - 4);

            // set footer size
            blockToFreeFoot->size_status = blockToFreeSize;

            // check the next block status
            blk_hdr *nextBlock = (blk_hdr*)((char*)blockToFree +
                            blockToFreeSize);

            if (nextBlock->size_status != 1) {
                nextBlock->size_status = (nextBlock->size_status) - 2;
            }

            // next block header
            int nextBlockData = nextBlock->size_status;

            // next block least sig bits
            int nextBlockLSB = nextBlockData % doubleWordSize;

            // see if block is at the end / if its free to coalate
            if ((nextBlockData != 1) && nextBlockLSB % 2 == 0) {
                    // size of next block
                    int nextBlockSize = nextBlockData - nextBlockLSB;

                    // Update size status with next block that was coalated
                    blockToFree->size_status = (blockToFree->size_status) +
                            nextBlockSize;

                    // get new footer at end of coalated block
                    blockToFreeFoot = (blk_hdr*)((char*)blockToFree +
                                    blockToFreeSize + nextBlockSize - 4);

                    // set accurate footer
                    blockToFreeFoot->size_status = blockToFreeSize +
                            nextBlockSize;
            }

            // check the block before, get size data and lsb
            // see if the block is free from lsb
            sizeData = blockToFree->size_status;
            LSBData = sizeData % doubleWordSize;
            blockToFreeSize = sizeData - LSBData;

            // if LSBData is 0 then previous block is free
            if (LSBData == 0) {
                    // get previous footer to get to header
                    blk_hdr *prevBlockFoot = (blk_hdr*)((char*)blockToFree - 4);

                    // how many bits to get to header of previous block
                    int sizeOfPrevious = prevBlockFoot->size_status;

                    // get prevous block
                    blk_hdr *previousBlockHead = (blk_hdr*)((char*)blockToFree -
                                    sizeOfPrevious);

                    // set size status to new falue with freed block
                    previousBlockHead->size_status = (previousBlockHead->
                                    size_status) + blockToFreeSize;

                    blockToFree = previousBlockHead;

                    blockToFreeFoot->size_status = blockToFreeSize +
                              sizeOfPrevious;
           }
    }



    return 0; 
}

/*
 * Function used to initialize the memory allocator
 * Not intended to be called more than once by a program
 * Argument - sizeOfRegion: Specifies the size of the chunk which needs to be allocated
 * Returns 0 on success and -1 on failure 
 */                    
int Init_Mem(int sizeOfRegion)
{                         
    int pagesize;
    int padsize;
    int fd;
    int alloc_size;
    void* space_ptr;
    blk_hdr* end_mark;
    static int allocated_once = 0;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: Init_Mem has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, 
                    fd, 0);
    if (MAP_FAILED == space_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
     allocated_once = 1;

    // for double word alignement and end mark
    alloc_size -= 8;

    // To begin with there is only one big free block
    // initialize heap so that first block meets 
    // double word alignement requirement
    first_blk = (blk_hdr*) space_ptr + 1;
    end_mark = (blk_hdr*)((void*)first_blk + alloc_size);
  
    // Setting up the header
    first_blk->size_status = alloc_size;

    // Marking the previous block as busy
    first_blk->size_status += 2;

    // Setting up the end mark and marking it as busy
    end_mark->size_status = 1;

    // Setting up the footer
    blk_hdr *footer = (blk_hdr*) ((char*)first_blk + alloc_size - 4);
    footer->size_status = alloc_size;
  
    return 0;
}

/* 
 * Function to be used for debugging 
 * Prints out a list of all the blocks along with the following information i
 * for each block 
 * No.      : serial number of the block 
 * Status   : free/busy 
 * Prev     : status of previous block free/busy
 * t_Begin  : address of the first byte in the block (this is where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block (as stored in the block header) (including the header/footer)
 */                     
void Dump_Mem() {                        
    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end = NULL;
    int t_size;

    blk_hdr *current = first_blk;
    counter = 1;

    int busy_size = 0;
    int free_size = 0;
    int is_busy = -1;

    fprintf(stdout, "************************************Block list***\
                    ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => busy block
            strcpy(status, "Busy");
            is_busy = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_busy = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "Busy");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "Free");
        }

        if (is_busy) 
            busy_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blk_hdr*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fprintf(stdout, "Total busy size = %d\n", busy_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", busy_size + free_size);
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fflush(stdout);

    return;
}
