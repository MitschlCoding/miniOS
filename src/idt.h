#ifndef IDT
#define IDT

/**
 * @file idt.h
 * @brief Header file for the Interrupt Descriptor Table (IDT) implementation.
 *
 * This header defines the structures and functions for managing the IDT,
 * which is used to handle interrupts in the miniOS kernel.
 */

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Represents an entry in the Interrupt Descriptor Table (IDT).
 */
typedef struct IdtEntryStruct {
  uint16_t lowerBase;         /**< The lower 16 bits of the base address of the interrupt handler. */
  uint16_t kernelCodeSegment; /**< The segment selector for the kernel code segment. */
  uint8_t zero;               /**< A reserved byte, always set to 0. */
  uint8_t typeAttribute;      /**< The type and attributes of the interrupt gate. */
  uint16_t higherBase;        /**< The upper 16 bits of the base address of the interrupt handler. */
} __attribute__((packed)) IdtEntry;

/**
 * @brief Represents the Interrupt Descriptor Table (IDT) descriptor.
 * @details This structure is used to load the IDT into the CPU.
 */
typedef struct IdtDescriptorStruct {
  uint16_t limit; /**< The size of the IDT in bytes. */
  uint32_t base;  /**< The base address of the IDT in memory. */
} __attribute__((packed)) IdtDescriptor;

/**
 * @brief Initializes the Interrupt Descriptor Table (IDT).
 *
 * This function sets up the IDT with the appropriate interrupt handlers
 * and loads it into the CPU.
 */
void idtInit();

/**
 * @brief Prints information about the IDT.
 *
 * This function iterates through the IDT entries and prints their
 * details, such as the interrupt vector number and the address of
 * the corresponding handler.
 */
void printIdtInfo();

/**
 * @brief Prints information about the IDT to the terminal.
 *
 * This function retrieves the IDT information from the CPU and prints it to the terminal
 * using the terminal output system.
 */
void printIdtInfoToTerminal();

#endif
