/**
 * @file snake.c
 * @brief Snake game implementation for miniOS
 */

#include "snake.h"
#include "printOS.h"
#include "keyboard.h"
#include "time.h"
#include "str.h"

#define BOARD_WIDTH 40
#define BOARD_HEIGHT 20
#define MAX_SNAKE_LENGTH 100

/**
 * @brief Represents a position on the game board.
 */
typedef struct {
    int x, y;
} Position;

/** 
 * @brief Represents the snake and its state.
 */
typedef struct {
    Position body[MAX_SNAKE_LENGTH]; /**< The body segments of the snake. */
    int length; /**< The current length of the snake. */
    int direction; /**< The current direction of the snake (0=up, 1=right, 2=down, 3=left). */
} Snake;

static Snake snake; // The snake object that holds the state of the game
static Position food; // The position of the food on the game board
static uint64_t lastUpdate = 0; // Last time the game was updated
static const uint64_t GAME_SPEED = 200; // milliseconds between moves
static bool gameOver = false; // Flag to indicate if the game is over

/**
 * @brief Moves the snake in the current direction.
 * 
 */
void moveSnake(void) {
    // Move body segments
    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }
    
    // Move head based on direction
    switch (snake.direction) {
        case 0: snake.body[0].y--; break; // up
        case 1: snake.body[0].x++; break; // right
        case 2: snake.body[0].y++; break; // down
        case 3: snake.body[0].x--; break; // left
    }
    
    // Check food collision
    if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
        snake.length++;
        if (snake.length < MAX_SNAKE_LENGTH) {
            generateFood();
        }
    }
}

/**
 * @brief Checks for collisions between the snake and the game boundaries or itself.
 * 
 * @return true if a collision is detected, false otherwise.
 */
bool checkCollision(void) {
    // Check walls
    if (snake.body[0].x <= 0 || snake.body[0].x >= BOARD_WIDTH - 1 ||
        snake.body[0].y <= 0 || snake.body[0].y >= BOARD_HEIGHT - 1) {
        return true;
    }
    
    // Check self collision
    for (int i = 1; i < snake.length; i++) {
        if (snake.body[0].x == snake.body[i].x && 
            snake.body[0].y == snake.body[i].y) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Generates a new food item on the game board.
 * 
 */
void generateFood(void) {
    bool validPosition = false;
    int attempts = 0;
    
    while (!validPosition && attempts < 100) {
        food.x = (timer_ms() % (BOARD_WIDTH - 2)) + 1;
        food.y = ((timer_ms() + attempts) % (BOARD_HEIGHT - 2)) + 1;
        
        validPosition = true;
        // Check if food spawns on snake
        for (int i = 0; i < snake.length; i++) {
            if (snake.body[i].x == food.x && snake.body[i].y == food.y) {
                validPosition = false;
                break;
            }
        }
        attempts++;
    }
}

/**
 * @brief Sets a character on the screen at the specified position.
 *
 * @param x The x-coordinate (column) on the screen.
 * @param y The y-coordinate (row) on the screen.
 * @param c The character to display.
 */
void screenSetChar(int x, int y, char c) {
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        uint8_t color = VGA_COLOR_WHITE | (VGA_COLOR_BLACK << 4); // White text on black background
        screenPutchar(c, color, x, y);
    }
}

/**
 * @brief Converts an integer to a string.
 *
 * @param num The integer to convert.
 * @param buffer The buffer to store the string representation.
 */
void simpleIntToString(int num, char* buffer) {
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    
    int i = 0;
    
    // Handle negative numbers
    bool negative = false;
    if (num < 0) {
        negative = true;
        num = -num;
    }
    
    // Convert digits
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    if (negative) {
        buffer[i++] = '-';
    }
    
    buffer[i] = '\0';
    
    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

/**
 * @brief Draws the current game state on the screen.
 * 
 */
void drawGame(void) {
    screenClear();
    
    // Draw border
    for (int x = 0; x < BOARD_WIDTH; x++) {
        screenSetChar(x, 0, '#');
        screenSetChar(x, BOARD_HEIGHT - 1, '#');
    }
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        screenSetChar(0, y, '#');
        screenSetChar(BOARD_WIDTH - 1, y, '#');
    }
    
    // Draw snake
    for (int i = 0; i < snake.length; i++) {
        char c = (i == 0) ? 'O' : 'o'; // Head vs body
        screenSetChar(snake.body[i].x, snake.body[i].y, c);
    }
    
    // Draw food
    screenSetChar(food.x, food.y, '*');
    
    // Draw score and controls on the side
    screenWriteLine("Score: ", 21);
    char scoreBuffer[20];
    simpleIntToString(snake.length - 3, scoreBuffer);
    screenWriteLine(scoreBuffer, 22);
}

// Docs see header file
void snakeGameInit(void) {
    screenClear();
    
    // Initialize snake in center
    snake.length = 3;
    snake.direction = 1; // moving right
    snake.body[0].x = BOARD_WIDTH / 2;
    snake.body[0].y = BOARD_HEIGHT / 2;
    snake.body[1].x = snake.body[0].x - 1;
    snake.body[1].y = snake.body[0].y;
    snake.body[2].x = snake.body[0].x - 2;
    snake.body[2].y = snake.body[0].y;
    
    // Place initial food
    generateFood();
    
    lastUpdate = timer_ms();
    gameOver = false;
}

//docs see header file
bool snakeGameUpdate(KeyCode key) {
    // Handle input with WASD
    switch (key) {
        case KEY_W:
            if (snake.direction != 2) snake.direction = 0; // up
            break;
        case KEY_S:
            if (snake.direction != 0) snake.direction = 2; // down
            break;
        case KEY_A:
            if (snake.direction != 1) snake.direction = 3; // left
            break;
        case KEY_D:
            if (snake.direction != 3) snake.direction = 1; // right
            break;
        case KEY_ESC:
            return true; // Exit game
        case KEY_ENTER:
            if (gameOver) return true; // Exit on game over
            break;
    }
    return false;
}

// docs see header file
void snakeGameTick(void) {
    if (gameOver) return;
    
    uint64_t currentTime = timer_ms();
    if (currentTime - lastUpdate >= GAME_SPEED) {
        moveSnake();
        if (checkCollision()) {
            gameOver = true;
            screenClear();
            screenWriteLine("Game Over!", 10);
            screenWriteLine("Press ENTER or ESC to return to terminal.", 11);
            return;
        }
        drawGame();
        lastUpdate = currentTime;
    }
}
