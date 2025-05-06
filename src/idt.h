#ifndef IDT
#define IDT
#include <stddef.h>
#include <stdint.h>

// struct for the interrupt descriptor table
typedef struct IdtEntryStruct {
  uint16_t lowerBase;         // lower base of the handler to call
  uint16_t kernelCodeSegment; // the segment code of our kernel
  uint8_t zero;               // random is always 0 thing, dont know why
  uint8_t typeAttribute;      // what type of interrupt this is, sets flags
  uint16_t higherBase;        // lower base of the handler to call
} __attribute__((packed)) IdtEntry;

// struct for the interrupt descriptor table descriptor
typedef struct IdtDescriptorStruct {
  uint16_t limit; // size of the table
  uint32_t base;  // base address of the table
} __attribute__((packed)) IdtDescriptor;

// initializes a interrupt descriptor table
void idtInit();

#endif
