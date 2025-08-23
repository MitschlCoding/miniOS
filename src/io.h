#ifndef IO_H
#define IO_H

/**
 * @file io.h
 * @brief Header file for input/output port operations.
 *
 * This header defines functions for reading and writing bytes and words
 * to and from hardware ports.
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
 * @brief Writes a word (16-bit) to a specified I/O port.
 *
 * @param port The I/O port number.
 * @param val The word value to write.
 */
void outw(uint16_t port, uint16_t val);

/**
 * @brief Reads a byte from a specified I/O port.
 *
 * @param port The I/O port number.
 * @return The byte value read from the I/O port.
 */
uint8_t inb(uint16_t port);

/**
 * @brief Reads a word (16-bit) from a specified I/O port.
 *
 * @param port The I/O port number.
 * @return The word value read from the I/O port.
 */
uint16_t inw(uint16_t port);

/**
 * @brief Waits for the I/O operation to complete.
 *
 * This function is used to insert a small delay, allowing the hardware
 * to process the previous I/O command.
 */
void ioWait(void);

#endif
