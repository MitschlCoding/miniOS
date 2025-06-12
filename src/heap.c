#include "heap.h"

void heap_init(void *start_addr, size_t total_size) {
  if (start_addr == NULL || total_size < sizeof(MemBlockHeader_t)) {
    // Invalid parameters, cannot initialize heap
    return;
  }
  heapTop = (MemBlockHeader_t *)start_addr;
  heapTop->nextFree = NULL;
  heapTop->dataSize = total_size - sizeof(MemBlockHeader_t);
  heapTop->isFree = true;
  heapTop->magicNumber = MEM_BLOCK_MAGIC_NUMBER;
}

void *mallocOS(size_t requested_size) {
  if (requested_size == 0) {
    // Invalid size
    return NULL;
  }
  if (heapTop == NULL) {
    // Heap not initialized
    return NULL;
  }
  // TODO: Write the code for allocating memory from the heap
  return NULL;
}

void freeOS(void *addr) {
  if (addr == NULL) {
    // Nothing to free
    return;
  }
  // TODO: Write the code for freeing memory in the heap
}
