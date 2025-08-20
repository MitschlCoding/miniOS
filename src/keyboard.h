#ifndef KEYBOARD_H
#define KEYBOARD_H

/**
 * @file
 * @brief Keyboard key codes and a simple key buffer API.
 *
 * @details
 * This header defines a subset of PC/AT Set 1 scan-code values as ::KeyCode
 * and declares a minimal FIFO buffer interface for key events, plus a helper
 * to map a key to a printable character.
 */

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Represents the key codes for keyboard input.
 */
typedef enum {
  KEY_NONE = 0x00,
  KEY_ESC = 0x01,
  KEY_1 = 0x02,
  KEY_2 = 0x03,
  KEY_3 = 0x04,
  KEY_4 = 0x05,
  KEY_5 = 0x06,
  KEY_6 = 0x07,
  KEY_7 = 0x08,
  KEY_8 = 0x09,
  KEY_9 = 0x0A,
  KEY_0 = 0x0B,
  KEY_BACKSPACE = 0x0E,
  KEY_Q = 0x10,
  KEY_W = 0x11,
  KEY_E = 0x12,
  KEY_R = 0x13,
  KEY_T = 0x14,
  KEY_Y = 0x15,
  KEY_U = 0x16,
  KEY_I = 0x17,
  KEY_O = 0x18,
  KEY_P = 0x19,
  KEY_ENTER = 0x1C,
  KEY_CTRL_L = 0x1D,
  KEY_A = 0x1E,
  KEY_S = 0x1F,
  KEY_D = 0x20,
  KEY_F = 0x21,
  KEY_G = 0x22,
  KEY_H = 0x23,
  KEY_J = 0x24,
  KEY_K = 0x25,
  KEY_L = 0x26,
  KEY_SHIFT_L = 0x2A,
  KEY_Z = 0x2C,
  KEY_X = 0x2D,
  KEY_C = 0x2E,
  KEY_V = 0x2F,
  KEY_B = 0x30,
  KEY_N = 0x31,
  KEY_M = 0x32,
  KEY_COMMA = 0x33,
  KEY_DOT = 0x34,
  KEY_SHIFT_R = 0x36,
  KEY_ALT_L = 0x38,
  KEY_SPACE = 0x39,
  KEY_F1 = 0x3B,
  KEY_F2 = 0x3C,
} KeyCode;

/**
 * @brief Initializes the key buffer.
 * @details This function sets up the key buffer to store keyboard input.
 * It should be called before using any key buffer functions.
 */
void keyBufferInit();

/**
 * @brief Puts a key code into the key buffer.
 * @param keycode The key code to be added to the buffer.
 * @details This function adds a key code to the key buffer.
 */
void keyBufferPut(KeyCode keycode);

/**
 * @brief Retrieves a key code from the key buffer.
 * @return KeyCode The key code retrieved from the buffer.
 */
KeyCode keyBufferGet();

/**
 * @brief Checks if the key buffer is empty.
 * @return true If the key buffer is empty.
 * @return false If the key buffer is not empty.
 */
bool keyBufferIsEmpty();

/**
 * @brief Get the Char From Key object
 * @param key The key code to convert to a character.
 * @return char The corresponding character, or 0 if the key code is invalid.
 */
char getCharFromKey(KeyCode key);

#endif
