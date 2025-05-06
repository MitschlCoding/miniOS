#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "printOS.h" // Your header for terminal I/O and error reporting
#include "shutdown.h"
#include "terminal.h"
#include <stddef.h>
#include <stdint.h>

void kernel_main(void) {

  // Setup the Global Descriptor Table
  gdtInit();

  // Setup Interrupt Descriptor Table
  idtInit();

  // Tell the os we will handle interrupts ourselfs
  __asm__ volatile("sti");

  // Initialize terminal or console interface
  screenInit();

  // Print the GdtInformation to confirm everything is runngin
  printGdtInfo();

  // wait for enter to be pressed
  while (1) {
    if (!keyBufferIsEmpty()) {
      KeyCode key = keyBufferGet();
      if (key == KEY_ENTER) {
        break;
      }
    }
  }
  screenClear();

  terminalInit();

  // infinite loop for os to run
  while (1) {
    if (!keyBufferIsEmpty()) {
      KeyCode key = keyBufferGet();
      // for now shutdown if esc is pressed
      keyPressedForTerminal(key);
    }
  }
}
