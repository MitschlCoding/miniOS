#include "audio.h"
#include "io.h"
#include "time.h"

/**
 * @brief Play sound using built-in speaker
 * @param frequency The frequency in Hz
 */
void play_sound(uint32_t frequency) {
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
 * @brief Stop all audio output (make it shut up)
 */
void nosound(void) {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}

/**
 * @brief Make a beep
 */
void beep(void) {
    play_sound(1000);
    timer_sleep_ms(200);
    nosound();
}
