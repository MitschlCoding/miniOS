#ifndef GDT_H
#define GDT_H

#include "printOS.h"
#include <stddef.h>
#include <stdint.h>

// Define the structure for a GDT entry (8 bytes each)
typedef struct {
  uint32_t base;
  uint32_t limit;
  uint8_t access_byte;
  uint8_t flags;
} __attribute__((packed)) Gdt;

// generate a gdtEntry from the struct above
void gdtEntry(uint8_t *target, Gdt source);

// gdt descriptor to pass to the cpu later
typedef struct {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) GdtDescriptor;

// reads the gdt info from the cpu and prints it
void printGdtInfo();

// initializes the gdt
void gdtInit();

#endif
