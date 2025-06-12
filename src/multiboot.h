#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stddef.h>
#include <stdint.h>

// Structure for the Multiboot information (passed in EBX)
typedef struct multiboot_info {
  uint32_t flags;       // Flags indicating which fields below are valid
  uint32_t mem_lower;   // Amount of lower memory (in KB)
  uint32_t mem_upper;   // Amount of upper memory (in KB)
  uint32_t boot_device; // BIOS boot device
  uint32_t cmdline;     // Physical address of command line string
  uint32_t mods_count;  // Number of modules loaded
  uint32_t mods_addr;   // Physical address of first module structure
  struct {
    uint32_t num;   // Size of symbol table entries
    uint32_t size;  // Number of symbol table entries
    uint32_t addr;  // Address of symbol table
    uint32_t shndx; // Index of string table in section headers
  } elf_sec;
  uint32_t mmap_length; // Total size of the memory map buffer
  uint32_t mmap_addr;   // Physical address of the memory map buffer
  // other fields not used
} __attribute__((packed)) multiboot_info_t;

// Structure for a single memory map entry
typedef struct memory_map_entry {
  uint32_t size;      // Size of this structure
  uint64_t base_addr; // Starting physical address
  uint64_t length;    // Length of the memory region in bytes
  uint32_t type;      // Type of region (1 = Available RAM, others)
} __attribute__((packed)) memory_map_entry_t;

void uint64ToHex(uint64_t value, char *buffer);
void printMultibootInfo(multiboot_info_t *mbi);
size_t checkMemoryMapForStack(multiboot_info_t *mbi);

// Define memory region types
#define MEMORY_REGION_AVAILABLE 1
// others not used atm

#endif
