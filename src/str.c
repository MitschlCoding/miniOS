#include "str.h"

size_t strlenOS(const char *str) {
  // count characters until \0 is found
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

void intToHex(uint32_t num, char *buffer) {
  buffer[0] = '0';
  buffer[1] = 'x';
  for (int i = 0; i < 8; i++) {
    uint8_t nibble = (num >> ((7 - i) * 4)) & 0xF;
    if (nibble < 10)
      buffer[2 + i] = '0' + nibble;
    else
      buffer[2 + i] = 'A' + nibble - 10;
  }
  buffer[10] = '\0';
}

// convert uint64_t to a hex string
void uint64ToHex(uint64_t value, char *buffer) {

  uint32_t high = (uint32_t)(value >> 32);
  uint32_t low = (uint32_t)(value & 0xFFFFFFFF);
  char lowStr[11]; // Buffer for lower part

  if (high > 0) {
    // If high part exists, print it first
    intToHex(high, buffer);
    // Find the end of the high part string
    char *endHigh = buffer;
    while (*endHigh) {
      endHigh++;
    }
    // Print the low part (padded)
    intToHex(low, lowStr);
    // Ensure low part is padded to 8 hex digits if high part exists
    int lowLen = 0;
    while (lowStr[lowLen])
      lowLen++;
    for (int i = 0; i < 8 - lowLen; i++) {
      *endHigh++ = '0'; // Add padding
    }
    // Copy low string
    char *lowPtr = lowStr;
    while (*lowPtr) {
      *endHigh++ = *lowPtr++;
    }
    *endHigh = '\0'; // Null terminate
  } else {
    // Only low part
    intToHex(low, buffer);
  }
}

int strcmpOS(char *a, char *b) {
  // while a has not ended and the current char of a is equal to the
  // coresponding in b, go forward
  while (*a != '\0' && *a == *b) {
    a++;
    b++;
  }
  // Return 0 if equal, difference of character codes otherwise
  return (unsigned char)*a - (unsigned char)*b;
}

char *trim(char *str) {
  if (str == NULL) {
    return NULL;
  }

  size_t len = strlenOS(str);

  // If the string is empty, there's nothing to trim
  if (len == 0) {
    return str;
  }

  // Start from the end of the string and move backwards
  // Check for common whitespace characters manually
  while (len > 0) {
    char c = str[len - 1];
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' ||
        c == '\v') {
      len--;
    } else {
      break; // Found a non-whitespace character
    }
  }

  // Null-terminate the string at the first non-space character from the end
  str[len] = '\0';

  return str;
}

// splits a c string based on a delimiter
size_t split(const char *str, char delimiter,
             char dest_buffer[NUM_SUBSTRINGS][LEN_SUBSTRINGS]) {
  if (str == NULL || dest_buffer == NULL) {
    return 0;
  }

  // Initialize the destination buffer to empty strings
  for (int i = 0; i < NUM_SUBSTRINGS; ++i) {
    if (LEN_SUBSTRINGS > 0) {
      dest_buffer[i][0] = '\0';
    }
  }

  int current_row = 0;
  const char *segment_start = str;
  const char *p = str;

  // Loop through the input string
  do {
    // A segment ends if a delimiter is found or the end of the string is
    // reached
    if (*p == delimiter || *p == '\0') {
      if (current_row >= NUM_SUBSTRINGS) {
        break; // Destination buffer is full
      }

      int current_col = 0;
      const char *char_to_copy = segment_start;

      // Copy characters from the segment to the current row in dest_buffer
      while (char_to_copy < p && current_col < LEN_SUBSTRINGS - 1) {
        dest_buffer[current_row][current_col++] = *char_to_copy++;
      }
      dest_buffer[current_row][current_col] =
          '\0'; // Null-terminate the copied segment

      current_row++;         // Move to the next row for the next segment
      segment_start = p + 1; // Next segment starts after the delimiter

      if (*p == '\0') {
        break; // End of the input string reached
      }
    }
    p++;
  } while (current_row < NUM_SUBSTRINGS && *(p - 1) != '\0');

  // Check *(p-1) because p has been incremented
  // Loop as long as buffer has space and string not ended

  return current_row; // Set the number of substrings found
}
