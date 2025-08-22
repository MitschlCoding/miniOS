#ifndef MULTIBOOT_H
#define MULTIBOOT_H

/** * @file multiboot.h
 * @brief Header file for Multiboot information structure and related functions.
 *
 * This header defines the Multiboot information structure used to pass
 * information from the bootloader to the kernel, including memory map,
 * command line arguments, and module information.
 */

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Multiboot information structure.
 *
 * This structure is used to pass information from the bootloader to the kernel.
 */
typedef struct multiboot_info {
  uint32_t flags;       /**< Flags indicating which fields below are valid */
  uint32_t mem_lower;   /**< Amount of lower memory (in KB) */
  uint32_t mem_upper;   /**< Amount of upper memory (in KB) */
  uint32_t boot_device; /**< BIOS boot device */
  uint32_t cmdline;     /**< Physical address of command line string */
  uint32_t mods_count;  /**< Number of modules loaded */
  uint32_t mods_addr;   /**< Physical address of first module structure */
  struct {
    uint32_t num;   /**< Size of symbol table entries */
    uint32_t size;  /**< Number of symbol table entries */
    uint32_t addr;  /**< Address of symbol table */
    uint32_t shndx; /**< Index of string table in section headers */
  } elf_sec;
  uint32_t mmap_length; /**< Total size of the memory map buffer */
  uint32_t mmap_addr;   /**< Physical address of the memory map buffer */
  // other fields not used
} __attribute__((packed)) multiboot_info_t;

/**
 * @brief Memory map entry structure.
 *
 * This structure represents a single entry in the memory map, describing
 * a region of memory and its attributes.
 */
typedef struct memory_map_entry {
  uint32_t size;      /**< Size of this structure */
  uint64_t base_addr; /**< Starting physical address */
  uint64_t length;    /**< Length of the memory region in bytes */
  uint32_t type;      /**< Type of region (1 = Available RAM, others) */
} __attribute__((packed)) memory_map_entry_t;

/** * @brief Converts an integer to a hexadecimal string.
 *
 * @param value The integer value to convert.
 * @param buffer The buffer to store the resulting hexadecimal string.
 */
void intToHex(uint32_t value, char *buffer);

/**
 * @brief Converts a 64-bit integer to a hexadecimal string.
 *
 * @param value The 64-bit integer value to convert.
 * @param buffer The buffer to store the resulting hexadecimal string.
 */
void uint64ToHex(uint64_t value, char *buffer);

/** 
 * @brief Prints the Multiboot information to the terminal.
 *
 * @param mbi Pointer to the Multiboot information structure.
 * @details This function prints various fields of the Multiboot information,
 * including memory size, command line, and module count.
 */
void printMultibootInfo(multiboot_info_t *mbi);

/**
 * @brief Checks the memory map for a suitable stack region.
 *
 * @param mbi Pointer to the Multiboot information structure.
 * @return size_t The size of the stack region found, or 0 if none found.
 */
size_t checkMemoryMapForStack(multiboot_info_t *mbi);

/**
 * @brief Calculates and stores the total available memory.
 *
 * @param mbi Pointer to the Multiboot information structure.
 * @details This function iterates through the memory map and sums up
 * all available memory regions (type 1) to calculate total system memory.
 */
void calculateTotalMemory(multiboot_info_t *mbi);

/**
 * @brief Gets the total available memory in bytes.
 *
 * @return uint64_t The total available memory in bytes.
 */
uint64_t getTotalMemoryBytes(void);

// Define memory region types
#define MEMORY_REGION_AVAILABLE 1
// others not used atm

#endif
