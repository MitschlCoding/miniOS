#include "str.h"

size_t strlenOS(const char *str) {
  // count characters until \0 is found
  size_t len = 0;
  while (str[len])
    len++;
  return len;
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
