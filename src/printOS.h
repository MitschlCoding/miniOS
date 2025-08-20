#ifndef PRINTOS_H
#define PRINTOS_H

/**
 * @file printOS.h
 * @brief Header file for printing text to the screen.
 */

#include <stddef.h>
#include <stdint.h>

// we are working with VGA to print text to the screen. For this we have a max
// width and max height, that is given by the VGA
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
// the memory address of the VGA that we need to write to
#define VGA_MEMORY 0xB8000

/**
 * @brief Writes a line of text to the specified line number on the screen.
 *
 * @param data The null-terminated string to write.
 * @param line_num The line number to write the string to (0-based).
 */
void screenWriteLine(const char *data, size_t line_num);

/**
 * @brief Sets a character at the specified position on the screen.
 *
 * @param c The character to set.
 * @param color The color attribute for the character.
 * @param x The x-coordinate (column) on the screen.
 * @param y The y-coordinate (row) on the screen.
 */
void screenPutchar(char c, uint8_t color, size_t x, size_t y);


/**
 * @brief Initializes the screen by setting up the VGA memory.
 *
 * This function clears the screen and prepares the VGA memory for
 * writing text.
 */
void screenInit();

/** 
 * @brief Clears the screen by filling it with spaces.
 *
 * This function fills the entire screen with spaces, effectively clearing
 * any previous content.
 */
void screenClear();

/**
 * @brief Enumeration of all available VGA colors.
 *
 * This enumeration defines the color values that can be used with the
 * VGA text mode.
 */
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
