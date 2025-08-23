#include "io.h"
#include <stdint.h>

void outb(uint16_t port, uint8_t val) {
  //write to port using asm
  __asm__ volatile("outb %0, %1"
                   : /* No outputs */
                   : "a"(val), "Nd"(port)
                   : "memory");
}

void outw(uint16_t port, uint16_t val) {
  //write word to port using asm
  __asm__ volatile("outw %0, %1"
                   : /* No outputs */
                   : "a"(val), "Nd"(port)
                   : "memory");
}

uint8_t inb(uint16_t port) {
  uint8_t result; // Variable to store the byte read from the port

  // read from port using asm
  __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port) : "memory");

  return result;
}

uint16_t inw(uint16_t port) {
  uint16_t result; // Variable to store the word read from the port

  // read word from port using asm
  __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port) : "memory");

  return result;
}

void ioWait(void) { 
  outb(0x80, 0); 
}
