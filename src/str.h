#ifndef STR
#define STR

#include <stddef.h>
#include <stdint.h>
#define NUM_SUBSTRINGS 64
#define LEN_SUBSTRINGS 64

// returns the length of a c string
size_t strlenOS(const char *str);

// compares two c strings
int strcmpOS(char *a, char *b);

// takes a c string and returns the same c string without trailing whitespace
char *trim(char *str);

// converts a int to a hex char*, that can be printed with terminalWriteLine
void intToHex(uint32_t num, char *buffer);

// convert a int64 to a hex char*
void uint64ToHex(uint64_t value, char *buffer);

// splits a c string based on a delimiter
size_t split(const char *str, char delimiter,
             char dest_buffer[NUM_SUBSTRINGS][LEN_SUBSTRINGS]);
#endif
