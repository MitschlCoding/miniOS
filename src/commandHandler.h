#ifndef COMMAND_HANDLER
#define COMMAND_HANDLER
#include "str.h"

#include <stddef.h>

void initCommands();
// this handles all commands and what to do when run
void commandHandler(char splitCommandBuffer[NUM_SUBSTRINGS][LEN_SUBSTRINGS],
                    size_t numSubstrings);

#endif
