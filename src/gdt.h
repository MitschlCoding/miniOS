#ifndef GDT_H
#define GDT_H

/**
 * @file gdt.h
 * @brief Header file for the Global Descriptor Table (GDT) implementation.
 */

#include "printOS.h"
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Represents a Global Descriptor Table (GDT) entry.
 * This structure holds the base and limit addresses for a segment. It is packed to ensure
 * that the structure is tightly packed without padding, which is important for the GDT format.
 */
typedef struct {
  uint32_t base; /**< The base address of the segment. */
  uint32_t limit; /**< The limit of the segment. */
  uint8_t access_byte; /**< The access byte for the segment, which defines its properties (e.g., present, ring level, executable). */
  uint8_t flags; /**< Flags for the segment, such as granularity and size. */
} __attribute__((packed)) Gdt;

/**
 * @brief Generates a GDT entry from the Gdt structure.
 *
 * @param target The target buffer to store the GDT entry.
 * @param source The source Gdt structure.
 */
void gdtEntry(uint8_t *target, Gdt source);

/**
 * @brief Represents a GDT descriptor.
 * This structure holds the description for our gdt needed to load the gdt.
 * It is packed to ensure that the structure is tightly packed without padding.
 */
typedef struct {
  uint16_t limit; /**< The size of the GDT in bytes. */
  uint32_t base; /**< The base address of the GDT in memory. */
} __attribute__((packed)) GdtDescriptor;

/**
 * @brief Prints the GDT information.
 *
 * This function retrieves the GDT information from the CPU and prints it to the terminal.
 */
void printGdtInfo();

/**
 * @brief Prints the GDT information to the terminal.
 *
 * This function retrieves the GDT information from the CPU and prints it to the terminal
 * using the terminal output system.
 */
void printGdtInfoToTerminal();

/**
 * @brief Initializes the Global Descriptor Table (GDT).
 *
 * This function sets up the GDT with the necessary entries and loads it into the CPU.
 */
void gdtInit();

#endif
