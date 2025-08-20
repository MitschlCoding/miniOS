#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/**
 * @file interrupts.h
 * @brief Interrupt handling functions and definitions.
 */

#include "register.h"

/**
 * @brief Handles an interrupt service routine (ISR).
 *
 * @param regs The CPU registers at the time of the interrupt.
 */
void isrHandler(registers_t *regs);

#endif
