#ifndef OUTB_H
#define OUTB_H

#include <stdint.h>

// this writes a uint8_t val to a given port
void outb(uint16_t port, uint8_t val);

// writes to some unused port, for the controller to have time to do the
// commands
void ioWait();

// reads a uint8_t from a given port
uint8_t inb(uint16_t port);

#endif
