#ifndef TERMINAL_H
#define TERMINAL_H

/**
 * @file terminal.h
 * @brief Terminal interface for miniOS.
 */

#include "keyboard.h"
#include "printOS.h"

/**
 * @brief Handles a key press event for the terminal.
 *
 * @param keycode The key code of the pressed key.
 * @details This function is called whenever a key is pressed in the terminal.
 * It processes the key input and updates the terminal state accordingly.
 */
void keyPressedForTerminal(KeyCode keycode);

/** 
 * @brief Initializes the terminal interface.
 *
 * @details This function sets up the terminal for input and output operations.
 * It should be called before using any terminal functions to ensure proper
 * initialization.
 */
void terminalInit();

/**
 * @brief Writes a line of text to the terminal.
 *
 * @param str The string to write to the terminal.
 * @details This function outputs a null-terminated string to the terminal,
 * handling any necessary formatting or color settings.
 */
void terminalWriteLine(char *str);

#endif
