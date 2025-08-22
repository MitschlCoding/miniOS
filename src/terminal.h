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

/**
 * @brief Shows the terminal interface.
 *
 * @details This function displays the terminal interface, typically called
 * when switching from another mode (such as visual mode) back to terminal mode.
 * It may clear the screen and redraw the terminal prompt or interface.
 */
void showTerminal(void);

/**
 * @brief Initializes and shows the terminal interface.
 *
 * @details This function combines terminal display initialization with showing
 * the terminal interface. It calls both showTerminal() and cmdLineInit() to
 * ensure the terminal is properly displayed with command line interface.
 * Used when switching back to terminal mode from visual modes.
 */
void initShowTerminal(void);

/**
 * @brief Marks the start of a command output sequence.
 *
 * @details This function should be called before executing a command that will
 * produce output. It ensures that the first line of output gets a '>' prefix
 * while subsequent lines get space prefixes for better readability.
 */
void terminalStartCommand(void);

/**
 * @brief Marks the end of a command output sequence.
 *
 * @details This function should be called after a command has finished
 * producing output. It resets the line prefix state for the next command.
 */
void terminalEndCommand(void);

#endif
