#ifndef HEAP
#define HEAP

/** 
 * @file heap.h
 * @brief Header file for the heap memory management implementation.
 *
 * This header defines the structures and functions for managing dynamic memory allocation
 * in the miniOS kernel using a simple stack-based approach.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Define the magic number for memory blocks to detect corruption
#define MEM_BLOCK_MAGIC_NUMBER 0xCAFEBABE
// Define the minimum size of a memory block header
#define MIN_DATA_SIZE 8

typedef struct MemBlockHeader MemBlockHeader_t;
/** 
 * @brief Represents a memory block header in the heap.
 *
 * This structure holds metadata for each memory block, including a pointer to the next free block,
 * the size of the data in the block, whether the block is free, and a magic number for integrity checks.
 */
struct MemBlockHeader {
  MemBlockHeader_t *nextFree; /**< Pointer to the next free memory block in the stack. */
  size_t dataSize;            /**< Size of the data in this block. */
  bool isFree;                /**< Is this block free? */
  uint32_t magicNumber;      /**< Magic number for detecting memory corruption. */
};

/**
 * @brief Pointer to the top of the heap stack.
 */
static MemBlockHeader_t *heapTop = NULL;

/**
 * @brief Initializes the heap memory management.
 *
 * @param start_addr The starting address of the heap.
 * @param total_size The total size of the heap.
 */
void heap_init(void *start_addr, size_t total_size);

/**
 * @brief Allocates a memory block of the specified size.
 *
 * @param requested_size The size of the memory block to allocate.
 * @return void* Pointer to the allocated memory block, or NULL if allocation failed.
 */
void *mallocOS(size_t requested_size);

/**
 * @brief Frees a previously allocated memory block.
 *
 * @param addr Pointer to the memory block to free.
 */
void freeOS(void *addr);

#endif
