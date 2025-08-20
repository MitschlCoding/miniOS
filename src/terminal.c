#include "terminal.h"
#include "commandHandler.h"
#include "printOS.h"
#include "shutdown.h"
#include "str.h"
#include <stddef.h>

// The cmd line that will be shown at the bottom of the terminal, so that the
// user can enter a command
#define MAX_LINE_WIDTH VGA_WIDTH
#define COMMAND_LINE_WIDTH (MAX_LINE_WIDTH + 64)
char cmdLine[COMMAND_LINE_WIDTH];
int16_t currCmdIndex = 1;

// A Ring for the terminal Output
#define TERMINAL_LINES_COUNT 32
int8_t terminalLineIndex = 0;
char terminalLineRing[TERMINAL_LINES_COUNT][MAX_LINE_WIDTH];

// Clears the terminal and sets the cursor to the top left corner
void terminalLinesInit() {
  for (size_t i = 0; i < TERMINAL_LINES_COUNT; i++) {
    for (size_t j = 0; j < TERMINAL_LINES_COUNT; j++) {
      terminalLineRing[i][j] = ' ';
    }
  }
  terminalLineIndex = 0;
}

// adds a string to the terminal with respect to newlines and terminal width
void terminalAddStr(char *str) {
  terminalLineIndex++;
  terminalLineIndex = terminalLineIndex % TERMINAL_LINES_COUNT;
  terminalLineRing[terminalLineIndex][0] = '>';
  size_t j = 1;
  for (size_t i = 0; i < strlenOS(str); i++) {
    j++;
    if (str[i] == '\n') {
      terminalLineIndex++;
      j = 1;
      continue;
    }
    if (j >= MAX_LINE_WIDTH) {
      terminalLineIndex++;
      terminalLineIndex = terminalLineIndex % TERMINAL_LINES_COUNT;
      terminalLineRing[terminalLineIndex][0] = ' ';
      terminalLineRing[terminalLineIndex][1] = ' ';
      j = 1;
    }
    terminalLineRing[terminalLineIndex][j] = str[i];
  }
  for (size_t i = j + 1; i < MAX_LINE_WIDTH + 2; i++) {
    terminalLineRing[terminalLineIndex][i] = ' ';
  }
}

// prints the command line at the bottom of the terminal
void cmdLineInit() {
  cmdLine[0] = '>';
  for (int i = 1; i < COMMAND_LINE_WIDTH; i++) {
    cmdLine[i] = ' ';
  }
  cmdLine[MAX_LINE_WIDTH] = '\0';
  screenWriteLine(cmdLine, VGA_HEIGHT - 1);
}

// prints whatever is written in the terminalLineRing to the screen
void showTerminal() {
  for (size_t i = 0; i < VGA_HEIGHT - 2; i++) {
    screenWriteLine(
        terminalLineRing[(terminalLineIndex - i) % TERMINAL_LINES_COUNT],
        VGA_HEIGHT - 3 - i);
  }
}

// initializes and shows the terminal interface
void initShowTerminal() {
  showTerminal();
  cmdLineInit();
}

void terminalWriteLine(char *str) {
  terminalAddStr(str);
  showTerminal();
}

// initializes the terminal
void terminalInit() {
  cmdLineInit();
  terminalLinesInit();
}

// Handler for user commands. This function is called whenever the user presses
// enter
void runCommand() {
  char splitCommandBuffer[NUM_SUBSTRINGS][LEN_SUBSTRINGS];
  char *command = cmdLine + 1;
  char *trimmedCommand = trim(command);

  // print into parts and send to commandHandler
  size_t numSubstrings = split(trimmedCommand, ' ', splitCommandBuffer);
  commandHandler(splitCommandBuffer, numSubstrings);
}

// This function is called whenever a key is pressed. It handles the input from
// the kernel
void keyPressedForTerminal(KeyCode keycode) {
  if (keycode == KEY_ENTER) {
    runCommand();
    currCmdIndex = 1;
    for (int8_t i = 1; i < VGA_WIDTH; i++) {
      cmdLine[i] = '\0';
    }
  } else if (keycode == KEY_BACKSPACE) {
    if (currCmdIndex > 1) {
      currCmdIndex--;
      cmdLine[currCmdIndex] = ' ';
    }
  } else {
    if (currCmdIndex >= COMMAND_LINE_WIDTH) {
      return;
    }
    char ch = getCharFromKey(keycode);
    if (ch == '\0') {
      return;
    }
    cmdLine[currCmdIndex] = ch;
    currCmdIndex++;
  }
  // TODO: print the full line not the start!
  screenWriteLine(cmdLine, VGA_HEIGHT - 1);
}
