#ifndef TERMINAL_H
#define TERMINAL_H

#include "keyboard.h"
#include "printOS.h"

// function that runs whenever a key was pressed
void keyPressedForTerminal(KeyCode keycode);

// initializes the terminal
void terminalInit();

void terminalWriteLine(char *str);

#endif
