#ifndef AUDIO_H
#define AUDIO_H

/**
 * @file audio.h
 * @brief Simple PC Speaker audio functionality.
 * 
 * This module provides basic functions to generate beeps using the PC Speaker
 * via PIT Channel 2, without interfering with the system timer (PIT Channel 0).
 */

#include <stdint.h>

/**
 * @brief Play a tone at the specified frequency
 * @param frequency The frequency in Hz
 */
void play_sound(uint32_t frequency);

/**
 * @brief Stop all audio output
 */
void nosound(void);

/**
 * @brief Play a simple beep sound
 */
void beep(void);

#endif // AUDIO_H
