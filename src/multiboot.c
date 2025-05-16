#include "multiboot.h" // Include the header with the struct definitions
#include "printOS.h"   // For terminalWriteLine, intToHex etc.
#include <stddef.h>    // For NULL
#include <stdint.h>

// Checks if the 'bit'-th bit is set in the 'flags' variable.
#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

// convert uint64_t to a hex string
void uint64ToHex(uint64_t value, char *buffer) {

  uint32_t high = (uint32_t)(value >> 32);
  uint32_t low = (uint32_t)(value & 0xFFFFFFFF);
  char lowStr[11]; // Buffer for lower part

  if (high > 0) {
    // If high part exists, print it first
    intToHex(high, buffer);
    // Find the end of the high part string
    char *endHigh = buffer;
    while (*endHigh) {
      endHigh++;
    }
    // Print the low part (padded)
    intToHex(low, lowStr);
    // Ensure low part is padded to 8 hex digits if high part exists
    int lowLen = 0;
    while (lowStr[lowLen])
      lowLen++;
    for (int i = 0; i < 8 - lowLen; i++) {
      *endHigh++ = '0'; // Add padding
    }
    // Copy low string
    char *lowPtr = lowStr;
    while (*lowPtr) {
      *endHigh++ = *lowPtr++;
    }
    *endHigh = '\0'; // Null terminate
  } else {
    // Only low part
    intToHex(low, buffer);
  }
}

// Prints the Multiboot information
void printMultibootInfo(multiboot_info_t *mbi) {
  char buffer[20]; // Buffer for hex conversions
  int line = 0;

  terminalWriteLine("--- Multiboot Information ---", line++);

  if (mbi == NULL) {
    terminalWriteLine("ERROR: Multiboot info pointer is NULL!", line++);
    return;
  }

  // 1. Print Flags
  intToHex(mbi->flags, buffer);
  terminalWriteLine("Flags:", line++);
  terminalWriteLine(buffer, line++);

  // 2. Print Memory Info (mem_lower, mem_upper) - Check Flag Bit 0
  if (CHECK_FLAG(mbi->flags, 0)) {
    intToHex(mbi->mem_lower, buffer);
    terminalWriteLine("Mem Lower (KB):", line++);
    terminalWriteLine(buffer, line++);
    intToHex(mbi->mem_upper, buffer);
    terminalWriteLine("Mem Upper (KB):", line++);
    terminalWriteLine(buffer, line++);
  } else {
    terminalWriteLine("Mem Lower/Upper: N/A (flag bit 0 not set)", line++);
    line++; // Skip lines for consistency
    line++;
    line++;
  }

  // 3. Print Boot Device - Check Flag Bit 1
  if (CHECK_FLAG(mbi->flags, 1)) {
    intToHex(mbi->boot_device, buffer);
    terminalWriteLine("Boot Device:", line++);
    terminalWriteLine(buffer, line++);
  } else {
    terminalWriteLine("Boot Device: N/A (flag bit 1 not set)", line++);
    line++;
  }

  // 4. Print Command Line Address - Check Flag Bit 2
  if (CHECK_FLAG(mbi->flags, 2)) {
    intToHex(mbi->cmdline, buffer);
    terminalWriteLine("Cmdline Addr:", line++);
    terminalWriteLine(buffer, line++);
  } else {
    terminalWriteLine("Cmdline Addr: N/A (flag bit 2 not set)", line++);
    line++;
  }

  // 5. Print Module Info - Check Flag Bit 3
  if (CHECK_FLAG(mbi->flags, 3)) {
    intToHex(mbi->mods_count, buffer);
    terminalWriteLine("Module Count:", line++);
    terminalWriteLine(buffer, line++);
    intToHex(mbi->mods_addr, buffer);
    terminalWriteLine("Module Addr:", line++);
    terminalWriteLine(buffer, line++);
  } else {
    terminalWriteLine("Modules: N/A (flag bit 3 not set)", line++);
    line++; // Skip lines
    line++;
    line++;
  }

  // 6. Print Memory Map Info - Check Flag Bit 6 (VERY IMPORTANT)
  if (CHECK_FLAG(mbi->flags, 6)) {
    intToHex(mbi->mmap_addr, buffer);
    terminalWriteLine("Mmap Addr:", line++);
    terminalWriteLine(buffer, line++);
    intToHex(mbi->mmap_length, buffer);
    terminalWriteLine("Mmap Length:", line++);
    terminalWriteLine(buffer, line++);

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
    terminalWriteLine("Memory Map: N/A (flag bit 6 not set!)", line++);
    line++;
    line++;
    line++;
  }

  terminalWriteLine("--- End Multiboot Info ---", line++);
  terminalWriteLine("Press enter to continue...", line + 1); // Leave space
}
