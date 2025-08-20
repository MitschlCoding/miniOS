#ifndef SNAKE_H
#define SNAKE_H

/**
 * @file snake.h
 * @brief Header file for the Snake game implementation.
 */

#include "keyboard.h"
#include <stdbool.h>

/**
 * @brief Initializes the Snake game.
 *
 * This function sets up the initial state of the Snake game,
 * including the position of the snake and the food.
 */
void snakeGameInit(void);

/**
 * @brief Updates the Snake game state.
 *
 * This function processes user input and updates the game state.
 *
 * @param key The key code of the pressed key.
 * @return true if the game should exit, false otherwise.
 */
bool snakeGameUpdate(KeyCode key);

/**
 * @brief Update function run for a gametick.
 *
 * This function updates the game state and checks for collisions, etc.
 */
void snakeGameTick(void);

#endif
