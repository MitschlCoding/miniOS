/**
 * @file kernel.c
 * @brief Main entry point for the kernel.
 *
 * This file contains the main function that initializes the kernel,
 * sets up the Global Descriptor Table (GDT), Interrupt Descriptor Table (IDT),
 * and handles basic terminal input/output.
 */

#include "commandHandler.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "multiboot.h"
#include "printOS.h" // Your header for terminal I/O and error reporting
#include "terminal.h"
#include "time.h"
#include "str.h"
#include "modeManager.h"
#include "snake.h"
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Kernel main entry point.
 *
 * @param mbi Pointer to the Multiboot information structure.
 */
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

  initCommands();
  pit_init(1000); // Initialize PIT with 1000 Hz
  modeManagerInit(); // Initialize mode manager
  terminalInit();

  // infinite loop for os to run
  while (1) {
    // Handle mode transitions
    handleModeTransitions();
    
    if (!keyBufferIsEmpty()) {
      KeyCode key = keyBufferGet();
      processModeInput(key);
    }
    
    // Update visual mode logic if in visual mode
    updateVisualMode();
  }
}
