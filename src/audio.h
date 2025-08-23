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
#include <stddef.h>

#define NOTE_C4    262    // C4 (Middle C) - 261.63 Hz
#define NOTE_CS4   277    // C#4/Db4 - 277.18 Hz
#define NOTE_D4    294    // D4 - 293.66 Hz
#define NOTE_DS4   311    // D#4/Eb4 - 311.13 Hz
#define NOTE_E4    330    // E4 - 329.63 Hz
#define NOTE_F4    349    // F4 - 349.23 Hz
#define NOTE_FS4   370    // F#4/Gb4 - 369.99 Hz
#define NOTE_G4    392    // G4 - 392 Hz
#define NOTE_GS4   415    // G#4/Ab4 - 415.30 Hz
#define NOTE_A4    440    // A4 (Concert pitch) - 440 Hz
#define NOTE_AS4   466    // A#4/Bb4 - 466.16 Hz
#define NOTE_B4    494    // B4 - 493.88 Hz

typedef struct {
    uint32_t frequency;
    uint32_t duration;
} Note;

/**
 * @brief Start playing a tone at the specified frequency
 * @param frequency The frequency in Hz
 */
void startSound(uint32_t frequency);

/**
 * @brief Stop all audio output
 */
void endSound(void);

/**
 * @brief Play a simple beep sound
 */
void beep(void);

/**
 * @brief Play a single musical note
 * @param note The note structure containing frequency and duration
 */
void playNote(Note note);

/**
 * @brief Play a sequence of musical notes
 * @param notes Array of Note structures
 * @param length Number of notes in the array
 */
void playSong(Note *notes, size_t length);

/**
 * @brief Play the Tetris theme song
 */
void tetrisSong(void);

#endif // AUDIO_H
