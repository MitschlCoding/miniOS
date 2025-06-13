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
  if (addr == NULL || heapTop == NULL) {
    return; // Nothing to free or heap not initialized
  }

  MemBlockHeader_t *block_to_free =
      (MemBlockHeader_t *)((char *)addr - sizeof(MemBlockHeader_t));

  if (block_to_free->magicNumber != MEM_BLOCK_MAGIC_NUMBER ||
      block_to_free->isFree) {
    // Invalid block or double-free, do nothing.
    return;
  }

  MemBlockHeader_t *current_free = heapTop;
  MemBlockHeader_t *prev_free = NULL;

  while (current_free != NULL && current_free < block_to_free) {
    prev_free = current_free;
    current_free = current_free->nextFree;
  }

  block_to_free->isFree = true;
  block_to_free->nextFree = current_free; // Link to the next free block

  // Link the previous free block to the block being freed
  if (prev_free != NULL) {
    prev_free->nextFree = block_to_free; // Insert in middle/end
  } else {
    heapTop = block_to_free; // This is now the new head of the free list
  }

  // COALESCE / MERGE with adjacent blocks

  // Merge with the NEXT block
  // Check if current_free is adjacent
  if (current_free != NULL) {
    uintptr_t end_of_freed_block = (uintptr_t)block_to_free +
                                   sizeof(MemBlockHeader_t) +
                                   block_to_free->dataSize;
    if (end_of_freed_block == (uintptr_t)current_free) {
      // They are adjacent. Merge current_free INTO block_to_free.
      block_to_free->dataSize +=
          sizeof(MemBlockHeader_t) + current_free->dataSize;
      block_to_free->nextFree =
          current_free->nextFree; // Bypass the now-merged block
    }
  }

  // Merge with the PREVIOUS block
  // Check if prev_free is adjacent
  if (prev_free != NULL) {
    uintptr_t end_of_prev_block =
        (uintptr_t)prev_free + sizeof(MemBlockHeader_t) + prev_free->dataSize;
    if (end_of_prev_block == (uintptr_t)block_to_free) {
      // They are adjacent. Merge block_to_free INTO prev_free.
      prev_free->dataSize += sizeof(MemBlockHeader_t) + block_to_free->dataSize;
      prev_free->nextFree =
          block_to_free->nextFree; // Bypass the now-merged block
      // No need to change 'block_to_free' as it's now considered part of
      // 'prev_free'
    }
  }
}
