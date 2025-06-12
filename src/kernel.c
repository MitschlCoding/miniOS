#include "commandHandler.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "multiboot.h"
#include "printOS.h" // Your header for terminal I/O and error reporting
#include "terminal.h"
#include <stddef.h>
#include <stdint.h>

void kernel_main(multiboot_info_t *mbi) {

  // Setup the Global Descriptor Table
  gdtInit();

  // Setup Interrupt Descriptor Table
  idtInit();

  // Tell the os we will handle interrupts ourselfs
  __asm__ volatile("sti");

  // Initialize terminal or console interface
  screenInit();

  // print out information about Multiboot
  printMultibootInfo(mbi); // Pass the pointer received from _start

  // wait for enter to be pressed
  while (1) {
    if (!keyBufferIsEmpty()) {
      KeyCode key = keyBufferGet();
      if (key == KEY_ENTER) {
        break;
      }
    }
  }
  screenClear(); // Clear screen after Multiboot info
  // Print the GdtInformation to confirm everything is runngin

  // print out information about Multiboot
  checkMemoryMapForStack(mbi); // Pass the pointer received from _start

  // wait for enter to be pressed
  while (1) {
    if (!keyBufferIsEmpty()) {
      KeyCode key = keyBufferGet();
      if (key == KEY_ENTER) {
        break;
      }
    }
  }
  screenClear(); // Clear screen after Multiboot info
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

  printIdtInfo();
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
  initCommands();

  // infinite loop for os to run
  while (1) {
    if (!keyBufferIsEmpty()) {
      KeyCode key = keyBufferGet();
      // for now shutdown if esc is pressed
      keyPressedForTerminal(key);
    }
  }
}
