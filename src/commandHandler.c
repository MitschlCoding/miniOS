#include "commandHandler.h"
#include "shutdown.h"
#include "str.h"
#include "terminal.h"
#include "modeManager.h"
#include "snake.h"

#define COMMAND_LIST_LENGTH 64

/**
 * @brief Represents a command in the command handler.
 *
 * This structure holds the name, help text, and handler function pointer
 * for a command.
 */
typedef struct {
  char *name; /**< The name of the command. */
  char *help; /**< A brief description of what the command does. */
  void (*handlerFuncPtr)(char[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *); /**< Pointer to the function that handles the command. */
} command;

/**
 * @brief Command list for the command handler.
 *
 * This array holds all the registered commands and their associated
 * handler functions. There can be a maximum of 64 commands.
 */
command commandList[64];


/**
 * @brief Handles the shutdown command.
 *
 * @param cmd The split command input.
 * @param buf The buffer to store the command output.
  * @details This function is called when the "shutdown" command is entered.
 */
void shutdownHandler(char cmd[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *buf) {
  systemShutdown();
}

/** * @brief Handles the help command.
 *
 * @param cmd The split command input.
 * @param buf The buffer to store the command output.
 * @details This function is called when the "help" command is entered.
 * It displays help information for all commands or a specific command if
 * provided. The help command prints all available commands and their
 * descriptions. If a specific command is requested, it prints the help
 * information for that command.
 */
void helpHandler(char cmd[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *buf) {

  if (strcmpOS(cmd[1], "") != 0) {
    // if a special command is requested
    for (int i = 0; i < COMMAND_LIST_LENGTH; i++) {
      if (commandList[i].name == NULL) {
        continue;
      }
      if (strcmpOS(cmd[1], commandList[i].name) == 0) {
        // found the command
        if (commandList[i].help != NULL) {
          terminalWriteLine(commandList[i].name);
          terminalWriteLine(commandList[i].help);
        } else {
          terminalWriteLine("No help available.");
        }
        return;
      }
    }
    return;
  } else {
    // if there is no command specified, print all commands
    char buffer[256];
    terminalWriteLine("--------Help----------");
    for (int i = 0; i < COMMAND_LIST_LENGTH; i++) {
      if (commandList[i].name == NULL) {
        continue;
      }
      concat("<<", commandList[i].name, buffer);
      concat(buffer, ">>", buffer);
      terminalWriteLine(buffer);
      if (i != COMMAND_LIST_LENGTH - 1) {
      }
    }
    terminalWriteLine("Type 'help <commandName>' for more information on a command.");
    terminalWriteLine("------------------");
  }
}

/**
 * @brief Handles the snake command.
 *
 * @param cmd The split command input.
 * @param buf The buffer to store the command output.
 * @details This function is called when the "snake" command is entered.
 * It starts the snake game by setting up the visual mode handlers and
 * switching to visual mode.
 */
void snakeHandler(char cmd[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *buf) {
  // Set up the snake game handlers
  setVisualModeHandlers(snakeGameUpdate, snakeGameTick);
  
  // Initialize the snake game
  snakeGameInit();
  
  // Switch to visual mode
  setCurrentMode(VISUAL_MODE);
}

// docs see header file
void initCommands() {
  commandList[0].name = "shutdown";
  commandList[0].help = "Shut down the system.";
  commandList[0].handlerFuncPtr = &shutdownHandler;

  commandList[1].name = "help";
  commandList[1].help = "Display all commands available.\nhelp <commandName> "
                        "displays the help information for that command.";
  commandList[1].handlerFuncPtr = &helpHandler;

  commandList[2].name = "snake";
  commandList[2].help = "Start the Snake game.\nUse WASD to control the snake, ESC to quit.";
  commandList[2].handlerFuncPtr = &snakeHandler;

  commandList[3].name = NULL;
  commandList[3].handlerFuncPtr = NULL;
}

// docs see header file
void commandHandler(char splitCommandBuffer[NUM_SUBSTRINGS][LEN_SUBSTRINGS],
                    size_t numSubstrings) {

  for (int i = 0; i < COMMAND_LIST_LENGTH; i++) {
    if (commandList[i].name == NULL) {
      continue;
    }
    char buffer[256];
    if (strcmpOS(splitCommandBuffer[0], commandList[i].name) == 0) {
      if (commandList[i].handlerFuncPtr == NULL) {
        continue;
      }
      commandList[i].handlerFuncPtr(splitCommandBuffer, buffer);
      return;
    }
  }

  terminalWriteLine("Command not recognized:");
  terminalWriteLine(splitCommandBuffer[0]);
}
