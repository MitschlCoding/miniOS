#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "register.h"

// interrupt service routine handler. Handles all hardware interrupts and is
// called from interrrupts_stubs.asm whenever a new interrupt happens
void isrHandler(registers_t *regs);

#endif
