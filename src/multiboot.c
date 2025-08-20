#include "multiboot.h" // Include the header with the struct definitions
#include "printOS.h"   // For terminalWriteLine, intToHex etc.
#include "str.h"
#include <stddef.h> // For NULL
#include <stdint.h>
#include <sys/types.h>

// Checks if the 'bit'-th bit is set in the 'flags' variable.
#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

extern char kernel_end[];

uintptr_t endOfKernelAdress = (uintptr_t)kernel_end;

// Prints the Multiboot information
void printMultibootInfo(multiboot_info_t *mbi) {
  char buffer[20]; // Buffer for hex conversions
  int line = 0;

  screenWriteLine("--- Multiboot Information ---", line++);

  if (mbi == NULL) {
    screenWriteLine("ERROR: Multiboot info pointer is NULL!", line++);
    return;
  }

  // 1. Print Flags
  intToHex(mbi->flags, buffer);
  screenWriteLine("Flags:", line++);
  screenWriteLine(buffer, line++);

  // 2. Print Memory Info (mem_lower, mem_upper) - Check Flag Bit 0
  if (CHECK_FLAG(mbi->flags, 0)) {
    intToHex(mbi->mem_lower, buffer);
    screenWriteLine("Mem Lower (KB):", line++);
    screenWriteLine(buffer, line++);
    intToHex(mbi->mem_upper, buffer);
    screenWriteLine("Mem Upper (KB):", line++);
    screenWriteLine(buffer, line++);
  } else {
    screenWriteLine("Mem Lower/Upper: N/A (flag bit 0 not set)", line++);
    line++; // Skip lines for consistency
    line++;
    line++;
  }

  // 3. Print Boot Device - Check Flag Bit 1
  if (CHECK_FLAG(mbi->flags, 1)) {
    intToHex(mbi->boot_device, buffer);
    screenWriteLine("Boot Device:", line++);
    screenWriteLine(buffer, line++);
  } else {
    screenWriteLine("Boot Device: N/A (flag bit 1 not set)", line++);
    line++;
  }

  // 4. Print Command Line Address - Check Flag Bit 2
  if (CHECK_FLAG(mbi->flags, 2)) {
    intToHex(mbi->cmdline, buffer);
    screenWriteLine("Cmdline Addr:", line++);
    screenWriteLine(buffer, line++);
  } else {
    screenWriteLine("Cmdline Addr: N/A (flag bit 2 not set)", line++);
    line++;
  }

  // 5. Print Module Info - Check Flag Bit 3
  if (CHECK_FLAG(mbi->flags, 3)) {
    intToHex(mbi->mods_count, buffer);
    screenWriteLine("Module Count:", line++);
    screenWriteLine(buffer, line++);
    intToHex(mbi->mods_addr, buffer);
    screenWriteLine("Module Addr:", line++);
    screenWriteLine(buffer, line++);
  } else {
    screenWriteLine("Modules: N/A (flag bit 3 not set)", line++);
    line++; // Skip lines
    line++;
    line++;
  }

  // 6. Print Memory Map Info - Check Flag Bit 6 (VERY IMPORTANT)
  if (CHECK_FLAG(mbi->flags, 6)) {
    intToHex(mbi->mmap_addr, buffer);
    screenWriteLine("Mmap Addr:", line++);
    screenWriteLine(buffer, line++);
    intToHex(mbi->mmap_length, buffer);
    screenWriteLine("Mmap Length:", line++);
    screenWriteLine(buffer, line++);

    /*
      terminalWriteLine("--- Memory Map Entries ---", line++);
      memory_map_entry_t* entry = (memory_map_entry_t*)mbi->mmap_addr;
      uintptr_t map_end = mbi->mmap_addr + mbi->mmap_length;
      while ((uintptr_t)entry < map_end) {
          char baseStr[17], lenStr[17], typeStr[11];
          uint64ToHex(entry->base_addr, baseStr);
          uint64ToHex(entry->length, lenStr);
          intToHex(entry->type, typeStr);

      positioning/line breaks terminalWriteLine(baseStr, line); // Need a way to
      write parts of a line
          // ... print length and type similarly ...
          line++;

          // Move to next entry
          entry = (memory_map_entry_t*)((uintptr_t)entry + entry->size +
      sizeof(uint32_t));
      }
      */

  } else {
    screenWriteLine("Memory Map: N/A (flag bit 6 not set!)", line++);
    line++;
    line++;
    line++;
  }

  // print information about end of kernel place
  intToHex(endOfKernelAdress, buffer);
  screenWriteLine("End of Kernel Address:", line++);
  screenWriteLine(buffer, line++);

  screenWriteLine("--- End Multiboot Info ---", line++);
  screenWriteLine("Press enter to continue...", line + 1); // Leave space
}

size_t checkMemoryMapForStack(multiboot_info_t *mbi) {

  if (mbi == NULL) {
    screenWriteLine("ERROR: Multiboot info pointer is NULL!", 0);
    return 0;
  }

  memory_map_entry_t *entry = (memory_map_entry_t *)mbi->mmap_addr;

  memory_map_entry_t *largest = entry;
  while (entry < (memory_map_entry_t *)(mbi->mmap_addr + mbi->mmap_length)) {

    // check if the entry is valid and has a part after our kernel
    if (entry->type != 1) {
      entry = (memory_map_entry_t *)((uintptr_t)entry + entry->size +
                                     sizeof(uint32_t));
      continue; // Skip non-available memory regions
    }
    if (entry->base_addr + entry->length <= endOfKernelAdress) {
      entry = (memory_map_entry_t *)((uintptr_t)entry + entry->size +
                                     sizeof(uint32_t));
      continue; // Skip regions that are completely before the kernel
    }
    if (entry->base_addr > endOfKernelAdress) {
      // if fully after the kernel, check if it is larger than the current
      // largest
      if (largest->length < entry->length) {
        largest = entry;
      }
    } else {
      // if not fully after the kernel, check if it has a part after the kernel
      if (largest->length - (largest->base_addr - endOfKernelAdress) <
          entry->length) {
        largest = entry;
      }
    }
    entry = (memory_map_entry_t *)((uintptr_t)entry + entry->size +
                                   sizeof(uint32_t));
  }

  if (largest == NULL) {
    screenWriteLine("No suitable memory region found for stack!", 0);
    return 0;
  } else {
    char buffer[20];
    uint64_t base_addr_found;
    if (largest->base_addr < endOfKernelAdress) {
      base_addr_found = endOfKernelAdress;
    } else {
      base_addr_found = largest->base_addr;
    }

    screenWriteLine("--- Stack Memory Region ---", 0);
    uint64ToHex(base_addr_found, buffer);
    screenWriteLine("--- Stack Memory Region Found ---", 1);
    screenWriteLine("Largest memory region after kernel:", 1);
    screenWriteLine("Base Address:", 2);
    intToHex(base_addr_found, buffer);
    screenWriteLine(buffer, 3);

    u_int64_t size_found;
    if (largest->base_addr < endOfKernelAdress) {
      size_found = largest->length - (endOfKernelAdress - largest->base_addr);
    } else {
      size_found = largest->length;
    }
    screenWriteLine("Size of region:", 4);
    intToHex(size_found, buffer);
    screenWriteLine(buffer, 5);

    screenWriteLine("--- End Stack Memory Region ---", 6);
    screenWriteLine("Press enter to continue...", 7);

    return size_found; // Return the size of the found region
  }
};
