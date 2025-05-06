#ifndef PRINTOS_H
#define PRINTOS_H

#include <stddef.h>
#include <stdint.h>

// we are working with VGA to print text to the screen. For this we have a max
// width and max height, that is given by the VGA
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
// the memory address of the VGA that we need to write to
#define VGA_MEMORY 0xB8000

// writes a line to a given line_number
void terminalWriteLine(const char *data, size_t line_num);

// sets up the VGA Memory
void screenInit();

// converts a int to a hex char*, that can be printed with terminalWriteLine
void intToHex(uint32_t num, char *buffer);

// clears the terminal
void screenClear();

// all available VGA colors
typedef enum vgaColor {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
} vgaColor;

#endif
