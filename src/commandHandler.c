#include "commandHandler.h"
#include "shutdown.h"
#include "str.h"
#include "terminal.h"
#include "modeManager.h"
#include "snake.h"
#include "time.h"
#include "gdt.h"
#include "idt.h"
#include "multiboot.h"
#include "art.h"
#include "audio.h"

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
  (void)cmd; // Suppress unused parameter warning
  (void)buf; // Suppress unused parameter warning
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
  (void)buf; // Suppress unused parameter warning

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
  (void)cmd; // Suppress unused parameter warning
  (void)buf; // Suppress unused parameter warning
  
  char buffer[256];
  formatUptime(buffer, sizeof(buffer));
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
  (void)cmd; // Suppress unused parameter warning
  (void)buf; // Suppress unused parameter warning
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
  (void)cmd; // Suppress unused parameter warning
  (void)buf; // Suppress unused parameter warning
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
  (void)cmd; // Suppress unused parameter warning
  (void)buf; // Suppress unused parameter warning
  // Set up the snake game handlers
  setVisualModeHandlers(snakeGameUpdate, snakeGameTick);
  
  // Initialize the snake game
  snakeGameInit();
  
  // Switch to visual mode
  setCurrentMode(VISUAL_MODE);
}

/**
 * @brief Handles the sysinfo command.
 *
 * @param cmd The split command input.
 * @param buf The buffer to store the command output.
 * @details This function displays comprehensive system information including
 * OS version, uptime and memory information.
 */
void sysinfoHandler(char cmd[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *buf) {
  (void)cmd; // Suppress unused parameter warning
  (void)buf; // Suppress unused parameter warning
  
  // Display cute axolotl ASCII art first
  printAxolotlArt();
  
  terminalWriteLine("=== System Information ===");
  terminalWriteLine("");
  
  // OS Information
  terminalWriteLine("Operating System: miniOS v1.0");
  terminalWriteLine("Kernel: 32-bit x86");
  terminalWriteLine("Architecture: i386");
  terminalWriteLine("");
  
  // Uptime information using the new formatUptime function
  char uptimeBuffer[256];
  formatUptime(uptimeBuffer, sizeof(uptimeBuffer));
  terminalWriteLine(uptimeBuffer);
  terminalWriteLine("");
  
  // Memory Information
  terminalWriteLine("Memory Information:");
  uint64_t totalMemory = getTotalMemoryBytes();
  if (totalMemory > 0) {
    char memBuffer[256];
    char numStr[32];
    
    // Convert bytes to MB for better readability
    uint64_t memoryMB = totalMemory / (1024 * 1024);
    uint64_t memoryKB = totalMemory / 1024;
    
    // Display total memory in MB
    concat("  Total RAM: ", "", memBuffer);
    uint64ToDecimalString(memoryMB, numStr);
    concat(memBuffer, numStr, memBuffer);
    concat(memBuffer, " MB (", memBuffer);
    uint64ToDecimalString(memoryKB, numStr);
    concat(memBuffer, numStr, memBuffer);
    concat(memBuffer, " KB)", memBuffer);
    terminalWriteLine(memBuffer);
    
    // Display in bytes for technical details
    concat("  Total RAM (bytes): ", "", memBuffer);
    uint64ToDecimalString(totalMemory, numStr);
    concat(memBuffer, numStr, memBuffer);
    concat(memBuffer, " bytes", memBuffer);
    terminalWriteLine(memBuffer);
  } else {
    terminalWriteLine("  Memory: Information not available");
  }
}

/**
 * @brief Handles the memory command.
 *
 * @param cmd The split command input.
 * @param buf The buffer to store the command output.
 * @details This function displays detailed memory information.
 */
void memoryHandler(char cmd[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *buf) {
  (void)cmd; // Suppress unused parameter warning
  (void)buf; // Suppress unused parameter warning
  
  terminalWriteLine("=== Memory Information ===");
  terminalWriteLine("");
  
  uint64_t totalMemory = getTotalMemoryBytes();
  if (totalMemory > 0) {
    char memBuffer[256];
    char numStr[32];
    
    // Convert bytes to MB and GB for better readability
    uint64_t memoryMB = totalMemory / (1024 * 1024);
    uint64_t memoryKB = totalMemory / 1024;
    uint64_t memoryGB = totalMemory / (1024 * 1024 * 1024);
    
    // Display total memory in different units
    if (memoryGB > 0) {
      concat("Total RAM: ", "", memBuffer);
      uint64ToDecimalString(memoryGB, numStr);
      concat(memBuffer, numStr, memBuffer);
      concat(memBuffer, " GB", memBuffer);
      terminalWriteLine(memBuffer);
    }
    
    concat("Total RAM: ", "", memBuffer);
    uint64ToDecimalString(memoryMB, numStr);
    concat(memBuffer, numStr, memBuffer);
    concat(memBuffer, " MB", memBuffer);
    terminalWriteLine(memBuffer);
    
    concat("Total RAM: ", "", memBuffer);
    uint64ToDecimalString(memoryKB, numStr);
    concat(memBuffer, numStr, memBuffer);
    concat(memBuffer, " KB", memBuffer);
    terminalWriteLine(memBuffer);
    
    concat("Total RAM: ", "", memBuffer);
    uint64ToDecimalString(totalMemory, numStr);
    concat(memBuffer, numStr, memBuffer);
    concat(memBuffer, " bytes", memBuffer);
    terminalWriteLine(memBuffer);
    
  } else {
    terminalWriteLine("Memory information not available");
    terminalWriteLine("This could mean:");
    terminalWriteLine("  - Multiboot info is not available");
    terminalWriteLine("  - Memory map flag is not set");
    terminalWriteLine("  - calculateTotalMemory() was not called");
  }
  terminalWriteLine("");
}

/**
 * @brief Handles the beep command.
 * @param cmd The split command input.
 * @param buf The buffer to store the command output.
 */
void beepHandler(char cmd[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *buf) {
  (void)cmd; // Suppress unused parameter warning
  (void)buf; // Suppress unused parameter warning
  
  terminalWriteLine("*BEEP*");
  beep();
}

/**
 * @brief Handle the music command to play musical melodies
 * @param cmd The split command input.
 * @param buf The buffer to store the command output.
 */
void musicHandler(char cmd[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *buf) {
  (void)cmd; // Suppress unused parameter warning
  (void)buf; // Suppress unused parameter warning
  
  terminalWriteLine("Playing Tetris theme song...");
  tetrisSong();
  terminalWriteLine("Song finished!");
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

  commandList[6].name = "sysinfo";
  commandList[6].help = "Display comprehensive system information including OS details, uptime, and features.";
  commandList[6].handlerFuncPtr = &sysinfoHandler;

  commandList[7].name = "memory";
  commandList[7].help = "Display detailed memory information including total RAM in different units.";
  commandList[7].handlerFuncPtr = &memoryHandler;

  commandList[8].name = "beep";
  commandList[8].help = "Play a system beep sound using the PC speaker.";
  commandList[8].handlerFuncPtr = &beepHandler;

  commandList[9].name = "music";
  commandList[9].help = "Play a melody using the PC speaker.";
  commandList[9].handlerFuncPtr = &musicHandler;

  commandList[10].name = NULL;
  commandList[10].handlerFuncPtr = NULL;
}

// docs see header file
void commandHandler(char splitCommandBuffer[NUM_SUBSTRINGS][LEN_SUBSTRINGS],
                    size_t numSubstrings) {
  (void)numSubstrings; // Suppress unused parameter warning

  for (int i = 0; i < COMMAND_LIST_LENGTH; i++) {
    if (commandList[i].name == NULL) {
      continue;
    }
    char buffer[256];
    if (strcmpOS(splitCommandBuffer[0], commandList[i].name) == 0) {
      if (commandList[i].handlerFuncPtr == NULL) {
        continue;
      }
      terminalStartCommand();
      commandList[i].handlerFuncPtr(splitCommandBuffer, buffer);
      terminalEndCommand();
      return;
    }
  }

  terminalStartCommand();
  terminalWriteLine("Command not recognized:");
  terminalWriteLine(splitCommandBuffer[0]);
  terminalEndCommand();
}
