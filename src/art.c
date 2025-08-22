#include "art.h"
#include "terminal.h"

/**
 * @brief ASCII art representation for miniOS.
 *
 * This ASCII art spells out "MOS" and is displayed at the start of sysinfo
 */
static char* mos_art[] = {
" _____ ______   ________  ________      ",
"|\\   _ \\  _   \\|\\   __  \\|\\   ____\\     ",
"\\ \\  \\\\\\__\\ \\  \\ \\  \\|\\  \\ \\  \\___|_    ",
" \\ \\  \\\\|__| \\  \\ \\  \\\\\\  \\ \\_____  \\   ",
"  \\ \\  \\    \\ \\  \\ \\  \\\\\\  \\|____|\\  \\  ",
"   \\ \\__\\    \\ \\__\\ \\_______\\____\\_\\  \\ ",
"    \\|__|     \\|__|\\|_______|\\_________\\",
"                            \\|_________|",
    NULL  // Null terminator for the array
};

void printAxolotlArt(void) {
    // Print each line of the MOS art
    for (int i = 0; mos_art[i] != NULL; i++) {
        terminalWriteLine(mos_art[i]);
    }
}
