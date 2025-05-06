#ifndef KEYBOARD_H
#define KEYBOARD_H

// TODO documentation
#include <stdbool.h>
#include <stdint.h>

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
  KEY_SPACE = 0x39
} KeyCode;

void keyBufferInit();
void keyBufferPut(KeyCode keycode);
KeyCode keyBufferGet();
bool keyBufferIsEmpty();

// returns a char for a given KeyCode
char getCharFromKey(KeyCode key);

#endif
