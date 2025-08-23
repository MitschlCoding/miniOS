#include "shutdown.h"
#include "io.h"
#include "printOS.h"
#include <stdint.h>

// Qemu expects the port 0x501 for shutdown, as defined in make file
#define QEMU_DEBUG_EXIT_PORT 0x501

void systemShutdown() {
  screenWriteLine("Attempting QEMU Shutdown via 0x501...", 0);

  // send 0 to QEMU_DEBUG_EXIT_PORT to signal that we want to shut down
  outb(QEMU_DEBUG_EXIT_PORT, 0);
  // If this worked, QEMU will exit. If it didn't, the code below might run.
  screenWriteLine("Error: Shutdown signal sent, should have exited.", 0);
  __asm__ volatile("cli; hlt");
}
