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
int16_t currCmdIndex = 2;

// A Ring for the terminal Output
#define TERMINAL_LINES_COUNT 128
int8_t terminalLineIndex = 0;
char terminalLineRing[TERMINAL_LINES_COUNT][MAX_LINE_WIDTH];

// Scroll offset for navigating terminal history
int8_t scrollOffset = 0;

// Track if we're on the first line of a command output
static bool firstLineOfCommand = true;

// Clears the terminal and sets the cursor to the top left corner
void terminalLinesInit() {
  for (size_t i = 0; i < TERMINAL_LINES_COUNT; i++) {
    for (size_t j = 0; j < MAX_LINE_WIDTH; j++) {
      terminalLineRing[i][j] = ' ';
    }
  }
  terminalLineIndex = 0;
}

// adds a string to the terminal with respect to newlines and terminal width
void terminalAddStr(char *str) {
  // Reset scroll when new content is added
  scrollOffset = 0;
  
  terminalLineIndex++;
  terminalLineIndex = terminalLineIndex % TERMINAL_LINES_COUNT;
  
  // Use '>' for first line of command, ' ' for subsequent lines
  if (firstLineOfCommand) {
    terminalLineRing[terminalLineIndex][0] = '>';
    firstLineOfCommand = false; // Mark that we're no longer on the first line
  } else {
    terminalLineRing[terminalLineIndex][0] = ' ';
  }
  
  size_t j = 1;
  for (size_t i = 0; i < strlenOS(str); i++) {
    // check if newline
    if (str[i] == '\n') {
      // Fill rest of current line with spaces
      for (size_t k = j; k < MAX_LINE_WIDTH; k++) {
        terminalLineRing[terminalLineIndex][k] = ' ';
      }
      // go to next line, reset column
      terminalLineIndex++;
      terminalLineIndex = terminalLineIndex % TERMINAL_LINES_COUNT;
      terminalLineRing[terminalLineIndex][0] = ' ';
      terminalLineRing[terminalLineIndex][1] = ' ';
      j = 2;
      continue;
    }
    // Check if we need to wrap to the next line
    if (j >= MAX_LINE_WIDTH) {
      terminalLineIndex++;
      terminalLineIndex = terminalLineIndex % TERMINAL_LINES_COUNT;
      terminalLineRing[terminalLineIndex][0] = ' ';
      terminalLineRing[terminalLineIndex][1] = ' ';
      j = 2;
    }
    // Copy the character to the current line
    terminalLineRing[terminalLineIndex][j] = str[i];
    j++;
  }
  // Fill rest of current line with spaces
  for (size_t i = j; i < MAX_LINE_WIDTH; i++) {
    terminalLineRing[terminalLineIndex][i] = ' ';
  }
}

// prints the command line at the bottom of the terminal
void cmdLineInit() {
  cmdLine[0] = '>';
  cmdLine[1] = '>';
  for (int i = 2; i < COMMAND_LINE_WIDTH; i++) {
    cmdLine[i] = ' ';
  }
  cmdLine[MAX_LINE_WIDTH] = '\0';
  screenWriteLine(cmdLine, VGA_HEIGHT - 1);
}

// prints whatever is written in the terminalLineRing to the screen
void showTerminal() {
  for (size_t i = 0; i < VGA_HEIGHT - 2; i++) {
    // Calculate the line index considering scroll offset
    int lineIndex = (terminalLineIndex - scrollOffset - i + TERMINAL_LINES_COUNT) % TERMINAL_LINES_COUNT;
    screenWriteLine(
        terminalLineRing[lineIndex],
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

// Marks the start of a command output sequence
void terminalStartCommand() {
  firstLineOfCommand = true;
}

// Marks the end of a command output sequence
void terminalEndCommand() {
  firstLineOfCommand = false;
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
  char *command = cmdLine + 2;
  char *trimmedCommand = trim(command);

  // print into parts and send to commandHandler
  size_t numSubstrings = split(trimmedCommand, ' ', splitCommandBuffer);
  commandHandler(splitCommandBuffer, numSubstrings);
  
  // Add empty line after command execution
  terminalAddStr("");
}

// This function is called whenever a key is pressed. It handles the input from
// the kernel
void keyPressedForTerminal(KeyCode keycode) {
  if (keycode == KEY_ENTER) {
    runCommand();
    currCmdIndex = 2;
    for (int8_t i = 2; i < VGA_WIDTH; i++) {
      cmdLine[i] = '\0';
    }
  } else if (keycode == KEY_BACKSPACE) {
    if (currCmdIndex > 2) {
      currCmdIndex--;
      cmdLine[currCmdIndex] = ' ';
    }
  } else if (keycode == KEY_ARROW_UP) {
    // Scroll up through terminal history
    if (scrollOffset < TERMINAL_LINES_COUNT - (VGA_HEIGHT - 2)) {
      scrollOffset++;
      showTerminal();
      screenWriteLine(cmdLine, VGA_HEIGHT - 1); // Redraw command line
    }
    return;
  } else if (keycode == KEY_ARROW_DOWN) {
    // Scroll down through terminal history
    if (scrollOffset > 0) {
      scrollOffset--;
      showTerminal();
      screenWriteLine(cmdLine, VGA_HEIGHT - 1); // Redraw command line
    }
    return;
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
