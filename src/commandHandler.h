#ifndef COMMAND_HANDLER
#define COMMAND_HANDLER

/**
 * @file
 * @brief Command handler, that handles commands input by the user into the terminal.
 *
 * @details
 * This header defines a command structure and a list of available commands,
 * along with their associated handler functions.
 */

#include "str.h"

#include <stddef.h>

/**
 * @brief Initialize the command handler.
 * @details
 * This function sets up the command list and registers the available commands.
 * It should be called before using the command handler to ensure all commands
 * are properly initialized.
 */
void initCommands();

/**
 * @brief Handles a command input by the user.
 * @param splitCommandBuffer A 2D array containing the split command input.
 * @param numSubstrings The number of substrings in the split command.
 * @details This function processes the command input, checks it against the
 * registered commands, and calls the appropriate handler function if a match
 * is found. If no match is found, it provides feedback to the user.
 */
void commandHandler(char splitCommandBuffer[NUM_SUBSTRINGS][LEN_SUBSTRINGS],
                    size_t numSubstrings);

#endif
