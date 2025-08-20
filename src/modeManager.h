#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

/**
 * @file modeManager.h
 * @brief Operating mode management for miniOS.
 */

#include "keyboard.h"
#include <stdbool.h>

/**
 * @brief Enumeration of available operating modes.
 */
typedef enum {
  TERMINAL_MODE,  /**< Terminal command line interface mode */
  VISUAL_MODE,    /**< Visual/game mode (e.g., Snake game) */
} OperatingMode;

/**
 * @brief Function pointer type for visual mode input handlers.
 *
 * @param key The key code of the pressed key.
 * @return bool True if the visual mode should exit, false to continue.
 */
typedef bool (*VisualModeInputHandler)(KeyCode key);

/**
 * @brief Function pointer type for visual mode update handlers.
 *
 * @details This function is called every frame to update the visual mode's logic.
 */
typedef void (*VisualModeUpdateHandler)(void);

/**
 * @brief Initializes the mode manager.
 *
 * @details This function sets up the initial operating mode and prepares
 * the mode management system for use.
 */
void modeManagerInit(void);

/**
 * @brief Gets the current operating mode.
 *
 * @return OperatingMode The current mode of the operating system.
 */
OperatingMode getCurrentMode(void);

/**
 * @brief Sets the current operating mode.
 *
 * @param mode The new operating mode to switch to.
 * @details This function changes the current operating mode and handles
 * any necessary mode transition logic.
 */
void setCurrentMode(OperatingMode mode);

/**
 * @brief Sets the visual mode handlers.
 *
 * @param inputHandler Function to handle keyboard input in visual mode.
 * @param updateHandler Function to update visual mode logic each frame.
 * @details This function sets the function pointers for the current visual
 * mode application. Call this before switching to VISUAL_MODE.
 */
void setVisualModeHandlers(VisualModeInputHandler inputHandler, VisualModeUpdateHandler updateHandler);

/**
 * @brief Checks if the operating mode has changed and handles transitions.
 *
 * @details This function should be called in the main loop to detect mode
 * changes and perform any necessary initialization when switching modes.
 * It automatically handles terminal initialization when switching to terminal mode.
 */
void handleModeTransitions(void);

/**
 * @brief Processes input for the current operating mode.
 *
 * @param key The key code of the pressed key.
 * @details This function routes keyboard input to the appropriate handler
 * based on the current operating mode.
 */
void processModeInput(KeyCode key);

/**
 * @brief Updates the current visual mode if active.
 *
 * @details This function calls the visual mode update handler if in visual mode.
 * Should be called in the main loop.
 */
void updateVisualMode(void);

#endif
