#ifndef time_h
#define time_h

/**
 * @file time.h
 * @brief Header file for time-related functions and definitions.
 *
 * This header defines functions for getting the current time in milliseconds
 * and for converting milliseconds to seconds.
 */

#include <stdint.h>

/**
 * @brief Initialize the Programmable Interrupt Timer (PIT).
 *
 * @param hz The desired frequency in Hertz.
 */
void pit_init(uint32_t hz);

/**
 * @brief Get the current timer ticks.
 *
 * @return uint64_t The current timer ticks.
 */
uint64_t timer_ticks(void); // raw ticks since pit_init()

/** * @brief Get the current time in milliseconds.
 *
 * @return uint64_t The current time in milliseconds since the PIT was initialized.
 */
uint64_t timer_ms(void);    // milliseconds since pit_init()

/**
 * @brief Sleep for a specified number of milliseconds.
 *
 * @param ms The number of milliseconds to sleep.
 */
void timer_sleep_ms(uint64_t ms);

/**
 * @brief Timer interrupt handler.
 *
 * This function is called when the timer interrupt occurs.
 */
void timer_irq(void);

#endif