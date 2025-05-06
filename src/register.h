#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

// This struct matches the order of registered pushed by isr_common_stub in
// interrupts_stubs.asm The C handler then receives a pointer to one of this
// struct, and can work with this afterwards
typedef struct {
  uint32_t ds; // Data segment selector
  uint32_t edi, esi, ebp, esp_useless, ebx, edx, ecx, eax; // registers common
  uint32_t int_no;   // Interrupt number signals the type of interrupt
  uint32_t err_code; // Error code
  uint32_t eip, cs, eflags, useresp, ss; // not sure
} registers_t;

#endif // REGISTERS_H
