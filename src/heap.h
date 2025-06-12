#ifndef HEAP
#define HEAP

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MEM_BLOCK_MAGIC_NUMBER 0xCAFEBABE

typedef struct MemBlockHeader MemBlockHeader_t;
struct MemBlockHeader {
  MemBlockHeader_t *nextFree; // Pointer to the next element in the stack
  size_t dataSize;            // Size of the data in this block
  bool isFree;                // Is this block free?
  uint32_t magicNumber;
};

static MemBlockHeader_t *heapTop = NULL; // Pointer to the top of the stack
                                         //
void heap_init(void *start_addr, size_t total_size);
void *mallocOS(size_t requested_size);
void freeOS(void *addr);

#endif
