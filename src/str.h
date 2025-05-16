#ifndef STR
#define STR

#include <stddef.h>

// returns the length of a c string
size_t strlenOS(const char *str);

// compares two c strings
int strcmpOS(char *a, char *b);

// takes a c string and returns the same c string without trailing whitespace
char *trim(char *str);

// splits a c string based on a delimiter
char **split(char *str, char delimiter);
#endif
