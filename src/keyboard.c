#include "keyboard.h"
#include "printOS.h"

#define KEY_BUFFER_SIZE 32
volatile KeyCode keyBuffer[KEY_BUFFER_SIZE];
volatile int8_t keyBufferReadIndex;
volatile int8_t keyBufferWriteIndex;

void keyBufferInit() {
  keyBufferReadIndex = 0;
  keyBufferWriteIndex = 0;
}

bool keyBufferIsEmpty() { return keyBufferReadIndex == keyBufferWriteIndex; }

void keyBufferPut(KeyCode keycode) {
  keyBuffer[keyBufferWriteIndex] = keycode;
  keyBufferWriteIndex = (keyBufferWriteIndex + 1) % KEY_BUFFER_SIZE;
}

KeyCode keyBufferGet() {
  if (keyBufferIsEmpty()) {
    return KEY_NONE;
  }
  uint8_t temp = keyBufferReadIndex;
  keyBufferReadIndex = (keyBufferReadIndex + 1) % KEY_BUFFER_SIZE;
  return keyBuffer[temp];
}

char getCharFromKey(KeyCode key) {
  switch (key) {
  // --- Zahlen ---
  case KEY_1:
    return '1';
  case KEY_2:
    return '2';
  case KEY_3:
    return '3';
  case KEY_4:
    return '4';
  case KEY_5:
    return '5';
  case KEY_6:
    return '6';
  case KEY_7:
    return '7';
  case KEY_8:
    return '8';
  case KEY_9:
    return '9';
  case KEY_0:
    return '0';

  // --- Obere Buchstabenreihe ---
  case KEY_Q:
    return 'q';
  case KEY_W:
    return 'w';
  case KEY_E:
    return 'e';
  case KEY_R:
    return 'r';
  case KEY_T:
    return 't';
  case KEY_Y:
    return 'y';
  case KEY_U:
    return 'u';
  case KEY_I:
    return 'i';
  case KEY_O:
    return 'o';
  case KEY_P:
    return 'p';

  // --- Mittlere Buchstabenreihe (Grundreihe) ---
  case KEY_A:
    return 'a'; // Jetzt eindeutig (0x1E) und direkt verwendbar
  case KEY_S:
    return 's';
  case KEY_D:
    return 'd';
  case KEY_F:
    return 'f';
  case KEY_G:
    return 'g';
  case KEY_H:
    return 'h';
  case KEY_J:
    return 'j';
  case KEY_K:
    return 'k';
  case KEY_L:
    return 'l';

  // --- Untere Buchstabenreihe ---
  case KEY_Z:
    return 'z';
  case KEY_X:
    return 'x';
  case KEY_C:
    return 'c';
  case KEY_V:
    return 'v';
  case KEY_B:
    return 'b';
  case KEY_N:
    return 'n';
  case KEY_M:
    return 'm';

  // --- Satzzeichen & Leertaste ---
  case KEY_COMMA:
    return ',';
  case KEY_DOT:
    return '.';
  case KEY_SPACE:
    return ' ';
  // case KEY_ENTER: return '\n'; // Alternative: Enter gibt Newline zurück

  // --- Nicht druckbare / Spezielle Tasten ---
  // Geben NULL zurück, da sie kein direktes Zeichen repräsentieren.
  // Die aufrufende Funktion (z.B. printKey) muss prüfen, ob NULL
  // zurückgegeben wurde und diese Tasten entsprechend behandeln.
  case KEY_ESC:       // 0x01
  case KEY_BACKSPACE: // 0x0E
  case KEY_ENTER:     // 0x1C (Hier als nicht-druckbar behandelt)
  case KEY_CTRL_L:    // 0x1D (Jetzt eindeutig)
  case KEY_SHIFT_L:   // 0x2A
  case KEY_SHIFT_R:   // 0x36
  case KEY_ALT_L:     // 0x38

  // --- Standardfall (Default) ---
  // Behandelt alle anderen (unbekannten) KeyCodes, die nicht explizit
  // aufgelistet sind.
  default:
    return NULL;
  }
}
