#include "commandHandler.h"
#include "shutdown.h"
#include "str.h"
#include "terminal.h"
#include "modeManager.h"
#include "snake.h"
#include "time.h"
#include "gdt.h"
#include "idt.h"

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
 * @brief Handles the uptime command.
 *
 * @param cmd The split command input.
 * @param buf The buffer to store the command output.
 * @details This function is called when the "uptime" command is entered.
 * It displays how long the system has been running since initialization.
 */
void uptimeHandler(char cmd[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *buf) {
  uint64_t milliseconds = timer_ms();
  uint64_t seconds = milliseconds / 1000;
  uint64_t minutes = seconds / 60;
  uint64_t hours = minutes / 60;
  uint64_t days = hours / 24;
  
  // Calculate remaining values
  seconds = seconds % 60;
  minutes = minutes % 60;
  hours = hours % 24;
  
  char buffer[256];
  char numStr[32];
  
  // Start with base message
  char* ptr = buffer;
  const char* msg = "System uptime: ";
  while (*msg) {
    *ptr++ = *msg++;
  }
  *ptr = '\0';
  
  if (days > 0) {
    uint64ToDecimalString(days, numStr);
    concat(buffer, numStr, buffer);
    if (days == 1) {
      concat(buffer, " day, ", buffer);
    } else {
      concat(buffer, " days, ", buffer);
    }
  }
  
  if (hours > 0 || days > 0) {
    uint64ToDecimalString(hours, numStr);
    concat(buffer, numStr, buffer);
    if (hours == 1) {
      concat(buffer, " hour, ", buffer);
    } else {
      concat(buffer, " hours, ", buffer);
    }
  }
  
  if (minutes > 0 || hours > 0 || days > 0) {
    uint64ToDecimalString(minutes, numStr);
    concat(buffer, numStr, buffer);
    if (minutes == 1) {
      concat(buffer, " minute, ", buffer);
    } else {
      concat(buffer, " minutes, ", buffer);
    }
  }
  
  uint64ToDecimalString(seconds, numStr);
  concat(buffer, numStr, buffer);
  if (seconds == 1) {
    concat(buffer, " second", buffer);
  } else {
    concat(buffer, " seconds", buffer);
  }
  
  terminalWriteLine(buffer);
}

/**
 * @brief Handles the gdt command.
 *
 * @param cmd The split command input.
 * @param buf The buffer to store the command output.
 * @details This function is called when the "gdt" command is entered.
 * It displays information about the Global Descriptor Table (GDT).
 */
void gdtHandler(char cmd[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *buf) {
  printGdtInfoToTerminal();
}

/**
 * @brief Handles the idt command.
 *
 * @param cmd The split command input.
 * @param buf The buffer to store the command output.
 * @details This function is called when the "idt" command is entered.
 * It displays information about the Interrupt Descriptor Table (IDT).
 */
void idtHandler(char cmd[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *buf) {
  printIdtInfoToTerminal();
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

  commandList[3].name = "uptime";
  commandList[3].help = "Display how long the system has been running since boot.";
  commandList[3].handlerFuncPtr = &uptimeHandler;

  commandList[4].name = "gdt";
  commandList[4].help = "Display Global Descriptor Table (GDT) information and verification status.";
  commandList[4].handlerFuncPtr = &gdtHandler;

  commandList[5].name = "idt";
  commandList[5].help = "Display Interrupt Descriptor Table (IDT) information and key interrupt handlers.";
  commandList[5].handlerFuncPtr = &idtHandler;

  commandList[6].name = NULL;
  commandList[6].handlerFuncPtr = NULL;
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
