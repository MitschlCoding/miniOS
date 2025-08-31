#include "audio.h"
#include "io.h"
#include "time.h"

/**
 * @brief Start playing sound using built-in speaker
 * @param frequency The frequency in Hz
 */
void startSound(uint32_t frequency) {
    uint32_t div;
    uint8_t tmp;

    // Set the PIT Channel 2 to the desired frequency
    div = 1193180 / frequency;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t)(div));
    outb(0x42, (uint8_t)(div >> 8));

    // And play the sound using the PC speaker
    tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}

/**
 * @brief Stop all audio output
 */
void endSound(void) {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}

/**
 * @brief Make a beep
 */
void beep(void) {
    startSound(1000);
    timer_sleep_ms(200);
    endSound();
}

void playNote(Note note) {
    if(note.frequency == 0) {
        timer_sleep_ms(note.duration);
        return;
    }
    startSound(note.frequency);
    timer_sleep_ms(note.duration);
    endSound();
}

void playSong(Note *notes, size_t length) {
    for (size_t i = 0; i < length; i++) {
        playNote(notes[i]);
    }
}

void tetrisSong(void) {
    Note melody[] = {
        // Line 1: E B C D C B A
        {NOTE_E4, 400},
        {NOTE_B4/2, 200},
        {NOTE_C4, 200},
        {NOTE_D4, 400},
        {NOTE_C4, 200},
        {NOTE_B4/2, 200},
        {NOTE_A4/2, 400},
        
        // Line 2: A C E D C B
        {NOTE_A4/2, 200},
        {NOTE_C4, 200},
        {NOTE_E4, 400},
        {NOTE_D4, 200},
        {NOTE_C4, 200},
        {NOTE_B4/2, 600},
        
        // Line 3: C D E C A A
        {NOTE_C4, 200},
        {NOTE_D4, 400},
        {NOTE_E4, 400},
        {NOTE_C4, 400},
        {NOTE_A4/2, 400},
        {NOTE_A4/2, 400},
        {0, 400},
        
        // Line 4: D F A G F E
        {NOTE_D4, 400},
        {NOTE_F4, 200},
        {NOTE_A4, 400},
        {NOTE_G4, 200},
        {NOTE_F4, 200},
        {NOTE_E4, 600},
        
        // Line 5: C E D C B
        {NOTE_C4, 200},
        {NOTE_E4, 400},
        {NOTE_D4, 200},
        {NOTE_C4, 200},
        {NOTE_B4/2, 400},
        
        // Line 6: B C D E C A A
        {NOTE_B4/2, 200},
        {NOTE_C4, 200},
        {NOTE_D4, 400},
        {NOTE_E4, 400},
        {NOTE_C4, 400},
        {NOTE_A4/2, 400},
        {NOTE_A4/2, 400},
    };
    size_t length = sizeof(melody) / sizeof(melody[0]);
    playSong(melody, length);
}