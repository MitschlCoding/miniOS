#include "outb.h"
#include <stdint.h>

void outb(uint16_t port, uint8_t val) {
  //write to port using asm
  __asm__ volatile("outb %0, %1"
                   : /* No outputs */
                   : "a"(val), "Nd"(port)
                   : "memory");
}

void ioWait() { outb(0x80, 0); }

uint8_t inb(uint16_t port) {
  uint8_t result; // Variable to store the byte read from the port

  // read from port using asm
  __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port) : "memory");

  return result;
}
