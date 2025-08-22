#include "printOS.h"
#include "outb.h"
#include "str.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Disable the cursor by setting bit 5 of the Cursor Start Register (0x0A) to 1
// This is done by writing to the CRTC (Cathode Ray Tube Controller) registers
// which control the display settings of the VGA text mode.
#define CRTC_INDEX_PORT 0x3D4
// The Data Port is used to read/write data to the CRTC registers
#define CRTC_DATA_PORT 0x3D5

void vga_disable_cursor() {
  // Select Cursor Start Register (Index 0x0A)
  outb(CRTC_INDEX_PORT, 0x0A);
  // Read the current value from the Data Port
  uint8_t current_value = inb(CRTC_DATA_PORT);

  // Set Bit 5 to disable the cursor (Value | 00100000 binary)
  uint8_t new_value = current_value | 0x20;

  // Select Cursor Start Register again 
  outb(CRTC_INDEX_PORT, 0x0A);
  // Write the new value (with bit 5 set) back to the Data Port
  outb(CRTC_DATA_PORT, new_value);
}

// converts two VGA Colors to the used uint8_t format.
static inline uint8_t vgaEntryColor(vgaColor foreground,
                                    enum vgaColor background) {
  return foreground | background << 4;
}

// converts a vgaEntryColor and a given char to a uint16_t representation of a
// colored char
static inline uint16_t vgaEntry(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint16_t)color << 8;
}

// the VGA Buffer we have to write to, in order to print text to the screen
uint16_t *screenBuffer = (uint16_t *)VGA_MEMORY;

// just clears the terminal when initialized
void screenInit() {
  screenClear();
  vga_disable_cursor();
}

// puts a char with a given color to a posion x,y with respects to boundaries
void screenPutchar(char c, uint8_t color, size_t x, size_t y) {
  if (x >= VGA_WIDTH || y >= VGA_HEIGHT) {
    return;
  }
  const size_t index = y * VGA_WIDTH + x;
  screenBuffer[index] = vgaEntry(c, color);
}

void screenWriteLine(const char *data, size_t line_num) {
  size_t len = strlenOS(data);
  uint8_t color = vgaEntryColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  for (size_t i = 0; i < len; i++) {
    // if width is to high this returns and doesnt try to print more characters
    if (i >= VGA_WIDTH) {
      return;
    }
    // put the char on the line
    screenPutchar(data[i], color, i, line_num);
  }
  for (size_t i = len; i < VGA_WIDTH; i++) {
    screenPutchar(' ', color, i, line_num);
  }
}

void screenClear() {
  uint8_t terminal_color = vgaEntryColor(VGA_COLOR_GREEN, VGA_COLOR_BLACK);

  // write a space to all the available places in the buffer
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      screenBuffer[index] = vgaEntry(' ', terminal_color);
    }
  }
};
