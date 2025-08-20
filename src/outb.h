#ifndef OUTB_H
#define OUTB_H

/**
 * @file outb.h
 * @brief Header file for input/output port operations.
 *
 * This header defines functions for writing and reading bytes to and from
 * hardware ports.
 */

#include <stdint.h>

/**
 * @brief Writes a byte to a specified I/O port.
 *
 * @param port The I/O port number.
 * @param val The byte value to write.
 */
void outb(uint16_t port, uint8_t val);

/**
 * @brief Waits for the I/O operation to complete.
 *
 * This function is used to insert a small delay, allowing the hardware
 * to process the previous I/O command.
 */
void ioWait();

/**
 * @brief Reads a byte from a specified I/O port.
 *
 * @param port The I/O port number.
 * @return The byte value read from the I/O port.
 */
uint8_t inb(uint16_t port);

#endif
