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

  // first fit allocation strategy
  MemBlockHeader_t *last = NULL;
  MemBlockHeader_t *current = heapTop;
  while (current != NULL) {
    if (current->isFree && current->dataSize >= requested_size) {
      // Found a suitable block
      size_t remainingSize = current->dataSize - requested_size;
      // if there is not surficient space for a new block after the requested
      // size
      if (remainingSize < sizeof(MemBlockHeader_t) + MIN_DATA_SIZE) {
        // allocate the entire block
        current->isFree = false;
        if (last != NULL) {
          last->nextFree = current->nextFree; // Remove current from free list
        } else {
          heapTop = current->nextFree; // Update heapTop if current is the first
        }
        return (
            void *)((char *)current +
                    sizeof(
                        MemBlockHeader_t)); // Return pointer to the data area
      } else {
        // create new free block after allocated memory
        MemBlockHeader_t *new_free_block =
            (void *)((uintptr_t)current + sizeof(MemBlockHeader_t) +
                     requested_size);
        new_free_block->nextFree = current->nextFree;
        new_free_block->dataSize = remainingSize - sizeof(MemBlockHeader_t);
        new_free_block->isFree = true;
        new_free_block->magicNumber = MEM_BLOCK_MAGIC_NUMBER;

        current->isFree = false;
        current->dataSize = requested_size;
        current->nextFree = NULL;

        if (last != NULL) {
          last->nextFree =
              new_free_block; // Link the new block to the free list
        } else {
          heapTop = new_free_block; // Update heapTop if current is the first
        }
        return (void *)((char *)current + sizeof(MemBlockHeader_t));
      }
    }
    last = current;
    current = current->nextFree;
  }

  return NULL;
}

void freeOS(void *addr) {
  if (addr == NULL) {
    // Nothing to free
    return;
  }

  // get the current block header
  MemBlockHeader_t *block =
      (MemBlockHeader_t *)((char *)addr - sizeof(MemBlockHeader_t));

  if (block->magicNumber != MEM_BLOCK_MAGIC_NUMBER) {
    // Invalid block, cannot free
    return;
  }
  if (block->isFree) {
    // Block is already free
    return;
  }
}
