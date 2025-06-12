#include "commandHandler.h"
#include "shutdown.h"
#include "str.h"
#include "terminal.h"

#define COMMAND_LIST_LENGTH 64

// a struct representing a command with a handlerFunction and a name that is
// checked against
typedef struct {
  char *name;
  char *help;
  // a function to be called with the given split command as a char[][] and a
  // return buffer as char*
  void (*handlerFuncPtr)(char[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *);
} command;

command commandList[64];

void shutdownHandler(char cmd[NUM_SUBSTRINGS][LEN_SUBSTRINGS], char *buf) {
  systemShutdown();
}

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
    terminalWriteLine("Help------------------");
    for (int i = 0; i < COMMAND_LIST_LENGTH; i++) {
      if (commandList[i].name == NULL) {
        continue;
      }
      terminalWriteLine(commandList[i].name);
      if (commandList[i].help != NULL) {
        terminalWriteLine(commandList[i].help);
      } else {
        terminalWriteLine("No help available.");
      }
      if (i != COMMAND_LIST_LENGTH - 1) {
      }
    }
    terminalWriteLine("------------------");
  }
}

void initCommands() {
  commandList[0].name = "shutdown";
  commandList[0].help = "Shut down the system.";
  commandList[0].handlerFuncPtr = &shutdownHandler;

  commandList[1].name = "help";
  commandList[1].help = "Display all commands available.\nhelp <commandName> "
                        "displays the help information for that command.";
  commandList[1].handlerFuncPtr = &helpHandler;

  commandList[2].name = NULL;
  commandList[2].handlerFuncPtr = NULL;
}

// this handles all commands and what to do when run
void commandHandler(char splitCommandBuffer[NUM_SUBSTRINGS][LEN_SUBSTRINGS],
                    size_t numSubstrings) {

  for (int i = 0; i < COMMAND_LIST_LENGTH; i++) {
    if (commandList[i].name == NULL) {
      continue;
    }
    // TODO: Think about if i want to use a buffer to return the string that
    // should be printed, or leave it to the program to print stuff
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
