#include "keyboard.h"
#include "io.h"
#include "printOS.h"
#include "register.h"
#include "shutdown.h"
#include "terminal.h"
#include "time.h"
#include <stddef.h>
#include <stdint.h>

// defines the command ports for programmable interrupts controllers (master,
// slave) and the end of interrupt value
#define PIC1_COMMAND_PORT 0x20
#define PIC2_COMMAND_PORT 0xA0 // For Slave PIC EOI
#define PIC_EOI 0x20

// Static variable to track extended key sequences (0xE0 prefix)
// Two-byte sequence expected for extended keys
static bool extended_key = false;

// send whenever a interrupt was handled for the controller to know that the
// next is wanted
void pic_send_eoi(uint8_t irq) {
  if (irq >= 8) {
    outb(PIC2_COMMAND_PORT, PIC_EOI); // EOI to Slave Command Port
  }
  outb(PIC1_COMMAND_PORT, PIC_EOI); // EOI to Master Command Port
}

void isrHandler(registers_t *regs) {

  // Handle interrupt based on number
  switch (regs->int_no) {
  case 0: // Divide By Zero
    screenWriteLine("!! DIVIDE BY ZERO !! System Halted.", 0);
    __asm__ volatile("cli; hlt");
    break;

  case 8: // Double Fault
    screenWriteLine("!!! DOUBLE FAULT !!! System Halted.", 0);
    __asm__ volatile("cli; hlt");
    break;

  case 13: // General Protection Fault
    screenWriteLine("!! GENERAL PROTECTION FAULT !! System Halted.", 0);
    // TODO: Print more debug info (err_code, EIP, CS etc. from regs)
    __asm__ volatile("cli; hlt");
    break;

  case 14: // Page Fault
    screenWriteLine("!! PAGE FAULT (Not Handled) !! System Halted.", 0);
    // TODO: Implement page fault handler
    __asm__ volatile("cli; hlt");
    break;

  case 32: // PIT timer tick (IRQ0 after remap)
      timer_irq();
      break;

  case 33: // Keyboard Interrupt (IRQ 1, after remap interrupt number 33)
  {
    uint8_t scancode = inb(0x60); // Read scan code of pressed key

    // Check for extended key prefix (0xE0)
    if (scancode == 0xE0) {
      extended_key = true;
      break; // Wait for the next byte
    }
    
    // Handle key release codes (high bit set)
    if (scancode & 0x80) {
      extended_key = false; // Reset extended key flag on key release
      break; // Ignore key release events
    }
    
    // Handle extended keys
    if (extended_key) {
      switch (scancode) {
        case 0x48: keyBufferPut(KEY_ARROW_UP); break;
        case 0x50: keyBufferPut(KEY_ARROW_DOWN); break;
        case 0x4B: keyBufferPut(KEY_ARROW_LEFT); break;
        case 0x4D: keyBufferPut(KEY_ARROW_RIGHT); break;
        default: break; // Ignore unhandled extended keys
      }
      extended_key = false;
    } else {
      // Handle normal keys
      keyBufferPut((KeyCode)scancode);
    }
  } break;

  default:
    break;
  }

  // Check if the interrupt number is in the range of hardware IRQs (32-47)
  if (regs->int_no >= 32 && regs->int_no < 48) {
    pic_send_eoi(regs->int_no - 32); // Calculate IRQ number and send EOI
  }
}
