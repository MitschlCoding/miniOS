#include "modeManager.h"
#include "terminal.h"
#include "keyboard.h"

static OperatingMode currentMode = TERMINAL_MODE;
static OperatingMode previousMode = TERMINAL_MODE;

// Function pointers for visual mode handlers, that are currently running
static VisualModeInputHandler currentInputHandler = NULL;
static VisualModeUpdateHandler currentUpdateHandler = NULL;

void modeManagerInit(void) {
    currentMode = TERMINAL_MODE;
    previousMode = TERMINAL_MODE;
    currentInputHandler = NULL;
    currentUpdateHandler = NULL;
}

OperatingMode getCurrentMode(void) {
    return currentMode;
}

void setCurrentMode(OperatingMode mode) {
    currentMode = mode;
}

void setVisualModeHandlers(VisualModeInputHandler inputHandler, VisualModeUpdateHandler updateHandler) {
    currentInputHandler = inputHandler;
    currentUpdateHandler = updateHandler;
}

void handleModeTransitions(void) {
    // Check if mode has changed and handle transitions
    if (currentMode != previousMode) {
        if (currentMode == TERMINAL_MODE) {
            initShowTerminal(); // Initialize and show terminal when switching to terminal mode
        }
        previousMode = currentMode; // Update previous mode
    }
}

void processModeInput(KeyCode key) {
    if (currentMode == TERMINAL_MODE) {
        // Handle terminal input
        keyPressedForTerminal(key);
    } else if (currentMode == VISUAL_MODE) {
        // Handle visual mode input using the current handler
        if (currentInputHandler != NULL) {
            if (currentInputHandler(key)) {
                currentMode = TERMINAL_MODE; // Return to terminal mode
            }
        }
    }
}

void updateVisualMode(void) {
    if (currentMode == VISUAL_MODE && currentUpdateHandler != NULL) {
        currentUpdateHandler();
    }
}
