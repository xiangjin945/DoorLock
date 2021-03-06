/**
 ****************************************************************************************
 *
 * @file kernel_mem.h
 *
 * @brief API for the heap management module.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef _KERNEL_MEM_H_
#define _KERNEL_MEM_H_

#include "rwip_config.h"     // IP configuration
#include <stdint.h>          // standard integer
#include <stdbool.h>         // standard includes
#include "kernel_config.h"       // kernel configuration

/**
 ****************************************************************************************
 * @defgroup MEM Memory
 * @ingroup KERNEL
 * @brief Heap management module.
 *
 * This module implements heap management functions that allow initializing heap,
 * allocating and freeing memory.
 *
 * @{
 ****************************************************************************************
 */

#if (KERNEL_MEM_RW)

// forward declarations
struct mblock_free;

/**
 ****************************************************************************************
 * @brief Heap initialization.
 *
 * This function performs the following operations:
 * - sanity checks
 * - check memory allocated is at least large enough to hold two block descriptors to hold
 * start and end
 * - initialize the first and last descriptors
 * - save heap into kernel environment variable.
 *
 * @param[in]     type      Memory type.
 * @param[in|out] heap      Heap pointer
 * @param[in]     heap_size Size of the heap
 *
 *
 ****************************************************************************************
 */
void kernel_mem_init(uint8_t type, uint8_t *heap, uint16_t heap_size);

/**
 ****************************************************************************************
 * @brief Allocation of a block of memory.
 *
 * Allocates a memory block whose size is size; if no memory is available return NULL
 *
 * @param[in] size Size of the memory area that need to be allocated.
 * @param[in] type Type of memory block
 *
 * @return A pointer to the allocated memory area.
 *
 ****************************************************************************************
 */
void *kernel_malloc(uint32_t size, uint8_t type);


/**
 ****************************************************************************************
 * @brief Check if it's possible to allocate a block of memory with a specific size.
 *
 * @param[in] size Size of the memory area that need to be allocated.
 * @param[in] type Type of memory block
 *
 * @return True if memory block can be allocated, False else.
 *
 ****************************************************************************************
 */
bool kernel_check_malloc(uint32_t size, uint8_t type);

/**
 ****************************************************************************************
 * @brief Freeing of a block of memory.
 *
 * Free the memory area pointed by mem_ptr : mark the block as free and insert it in
 * the pool of free block.
 *
 * @param[in] mem_ptr Pointer to the memory area that need to be freed.
 *
 ****************************************************************************************
 */
void kernel_free(void *mem_ptr);


/**
 ****************************************************************************************
 * @brief Check if current heap is empty or not (not used)
 *
 * @param[in] type Type of memory heap block
 *
 * @return true if heap not used, false else.
 ****************************************************************************************
 */
bool kernel_mem_is_empty(uint8_t type);



/**
 ****************************************************************************************
 * @brief Check if current pointer is free or not
 *
 * @param[in] mem_ptr pointer to a memory block
 *
 * @return true if already free, false else.
 ****************************************************************************************
 */
bool kernel_is_free(void *mem_ptr);

#if (KERNEL_PROFILING)

/**
 ****************************************************************************************
 * @brief Retrieve memory usage of selected heap.
 *
 * @param[in] type Type of memory heap block
 *
 * @return current memory usage of current heap.
 ****************************************************************************************
 */
uint16_t kernel_get_mem_usage(uint8_t type);


/**
 ****************************************************************************************
 * @brief Retrieve max memory usage of all heap.
 * This command also resets max measured value.
 *
 * @return max memory usage of all heap.
 ****************************************************************************************
 */
uint32_t kernel_get_max_mem_usage(void);

#endif // (KERNEL_PROFILING)




#elif (KERNEL_MEM_LINUX)
// Wrappers to Linux mem functions here

#include <linux/slab.h>

__INLINE void *kernel_malloc(uint32_t size)
{
    return kmalloc(size, GFP_KERNEL);
}

__INLINE void kernel_free (void *mem_ptr)
{
    kfree(mem_ptr);
}

#elif (KERNEL_MEM_LIBC)
// Wrapper to lib C mem functions here
#include <stdlib.h>

__INLINE void *kernel_malloc(uint32_t size)
{
    return malloc(size);
}

__INLINE void kernel_free(void *mem_ptr)
{
    free(mem_ptr);
}
#else
void *kernel_malloc(uint32_t size, uint8_t type);
void kernel_free(void *mem_ptr);
bool kernel_check_malloc(uint32_t size, uint8_t type);
bool kernel_is_free(void *mem_ptr);
#endif // KERNEL_MEM_RW

///@} MEM

#endif // _KERNEL_MEM_H_

