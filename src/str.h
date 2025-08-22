#ifndef STR
#define STR

/**
 * @file str.h
 * @brief String manipulation functions for miniOS.
 */

#include <stddef.h>
#include <stdint.h>

// Define constants for the maximum number of substrings and their length
#define NUM_SUBSTRINGS 64
#define LEN_SUBSTRINGS 64

/**
 * @brief Returns the length of a C string.
 *
 * @param str The string to measure.
 * @return size_t The length of the string.
 */
size_t strlenOS(const char *str);

/**
 * @brief Compares two C strings.
 *
 * @param a The first string.
 * @param b The second string.
 * @return int A negative value if a < b, a positive value if a > b, and 0 if they are equal.
 */
int strcmpOS(char *a, char *b);

/**
 * @brief Trims trailing whitespace from a C string.
 *
 * @param str The string to trim.
 * @return char* Pointer to the trimmed string.
 */
char *trim(char *str);

/**
 * @brief Converts an integer to a hexadecimal string.
 *
 * @param num The integer value to convert.
 * @param buffer The buffer to store the resulting hexadecimal string.
 */
void intToHex(uint32_t num, char *buffer);

/**
 * @brief Converts a 64-bit integer to a hexadecimal string.
 *
 * @param value The 64-bit integer value to convert.
 * @param buffer The buffer to store the resulting hexadecimal string.
 */
void uint64ToHex(uint64_t value, char *buffer);

/**
 * @brief Splits a C string into substrings based on a delimiter.
 *
 * @param str The string to split.
 * @param delimiter The character used to split the string.
 * @param dest_buffer A 2D array to store the resulting substrings.
 * @return size_t The number of substrings created.
 */
size_t split(const char *str, char delimiter,
             char dest_buffer[NUM_SUBSTRINGS][LEN_SUBSTRINGS]);

/**
 * @brief Concatenates two C strings into a buffer.
 *
 * @param str1 The first string.
 * @param str2 The second string.
 * @param buffer The buffer to store the concatenated result.
 * @return char* Pointer to the resulting concatenated string in the buffer.
 */
char* concat(const char *str1, const char *str2, char *buffer);

/**
 * @brief Converts a 64-bit unsigned integer to a decimal string.
 *
 * @param num The 64-bit unsigned integer to convert.
 * @param buffer The buffer to store the resulting decimal string.
 */
void uint64ToDecimalString(uint64_t num, char *buffer);

/**
 * @brief Converts a 32-bit unsigned integer to a decimal string.
 *
 * @param num The 32-bit unsigned integer to convert.
 * @param buffer The buffer to store the resulting decimal string.
 */
void uint32ToDecimalString(uint32_t num, char *buffer);

/**
 * @brief Converts a 16-bit unsigned integer to a decimal string.
 *
 * @param num The 16-bit unsigned integer to convert.
 * @param buffer The buffer to store the resulting decimal string.
 */
void uint16ToDecimalString(uint16_t num, char *buffer);

/**
 * @brief Converts a signed integer to a decimal string.
 *
 * @param num The signed integer to convert.
 * @param buffer The buffer to store the resulting decimal string.
 */
void intToDecimalString(int num, char *buffer);

#endif
