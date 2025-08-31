#include "gdt.h"
#include "printOS.h"
#include "str.h"
#include "terminal.h"
#include <sys/types.h>

// Define the size of the GDT in bytes
#define GDT_SIZE (3 * 8)
/**
 * @brief Represents the Global Descriptor Table (GDT) in memory.
 *
 * This array holds the GDT entries, including the null segment, code segment,
 * and data segment.
 */
uint8_t gdt[GDT_SIZE];

/**
 * @brief Represents the GDT descriptor.
 *
 * This structure holds the description for our gdt needed to load the gdt.
 */
GdtDescriptor gdtDesc;

// documentation see gdt.h
void gdtEntry(uint8_t *target, GdtEntry source) {
  // Encode the limit (split into low and high)
  target[0] = source.limit & 0xFF;
  target[1] = (source.limit >> 8) & 0xFF;
  target[6] = (source.limit >> 16) & 0x0F;
  // Encode the base address (split into parts)
  target[2] = source.base & 0xFF;
  target[3] = (source.base >> 8) & 0xFF;
  target[4] = (source.base >> 16) & 0xFF;
  target[7] = (source.base >> 24) & 0xFF;
  // Encode the access byte
  target[5] = source.access_byte;
  // Encode the flags (placed in the high nibble of byte 6)
  target[6] |= (source.flags << 4);
}

// Set up the GDT with a null entry, a code segment and a data segment.
void gdtInit() {
  // Define a null Entry that is required
  GdtEntry nullEntry = {.base = 0, .limit = 0, .access_byte = 0, .flags = 0};

  // generate a code segment with maximum size at the base 0 and with these
  // settings
  // - Access: present, ring0, executable, readable
  // - Flags: 4k granularity and 32 bit mode
  GdtEntry codeEntry = {
      .base = 0, .limit = 0xFFFFF, .access_byte = 0x9A, .flags = 0x0C};

  // generate a data segment with maximum size at the base 0 and with these
  // settings
  // - Access: present, ring0, writable
  // - Flags: 4k granularity and 32 bit mode
  GdtEntry dataEntry = {
      .base = 0, .limit = 0xFFFFF, .access_byte = 0x92, .flags = 0x0C};

  // Create the 3 entries in our GDT array.
  gdtEntry(&gdt[0], nullEntry);
  gdtEntry(&gdt[8], codeEntry);
  gdtEntry(&gdt[16], dataEntry);

  // Set up the GDT descriptor with the proper limit and base address.
  gdtDesc.limit = sizeof(gdt) - 1;
  gdtDesc.base = (uint32_t)&gdt;

  // Use inline assembly to load the new GDT.
  __asm__ volatile("lgdt (%0)" : : "r"(&gdtDesc));

  // Perform a far jump, to load the gdt and update the code Segment
  __asm__ volatile(
      "mov $0x10, %%ax\n" // 0x10 is the selector for the data segment (second
                          // entry: 0x08 for code, 0x10 for data)
      "mov %%ax, %%ds\n"
      "mov %%ax, %%es\n"
      "mov %%ax, %%fs\n"
      "mov %%ax, %%gs\n"
      "mov %%ax, %%ss\n"
      "jmp $0x08, $.reload\n" // Far jump: 0x08 is the selector for the code
                              // segment
      ".reload:\n"
      :
      :
      : "ax");
}

// prints the info about the gdt
void printGdtInfo() {
  // struct to load data into
  GdtDescriptor currentGdt;

  // read the gdt descriptor
  __asm__ volatile("sgdt %0" : "=m"(currentGdt));

  char expectedLimitStr[11];
  char loadedLimitStr[11];
  char expectedBaseStr[11];
  char loadedBaseStr[11];

  // Convert the expected and loaded limit values to hex strings
  intToHex(gdtDesc.limit, expectedLimitStr);
  intToHex(currentGdt.limit, loadedLimitStr);

  uint8_t line = 0;

  screenWriteLine("--- GDT Information ---", line++);

  // Print expected and loaded limit values
  screenWriteLine("Expected Limit:", line++);
  screenWriteLine(expectedLimitStr, line++);
  screenWriteLine("Loaded Limit:", line++);
  screenWriteLine(loadedLimitStr, line++);

  // Convert the expected and loaded base addresses to hex strings
  intToHex((uint32_t)gdt, expectedBaseStr);
  intToHex(currentGdt.base, loadedBaseStr);

  // Print expected and loaded base addresses
  screenWriteLine("Expected Base:", line++);
  screenWriteLine(expectedBaseStr, line++);
  screenWriteLine("Loaded Base:", line++);
  screenWriteLine(loadedBaseStr, line++);

  // Compare the expected and loaded values
  if (gdtDesc.limit == currentGdt.limit && currentGdt.base == (uint32_t)gdt) {
    screenWriteLine("GDT loaded correctly!", line++);
  } else {
    screenWriteLine("GDT load verification failed!", line++);
  }
  screenWriteLine("--- End GDT Info ---", line++);
  line++;
  screenWriteLine("Press enter to continue...", line++);
}

// prints the info about the gdt to terminal (for command use)
void printGdtInfoToTerminal() {
  // struct to load data into
  GdtDescriptor currentGdt;

  // read the gdt descriptor
  __asm__ volatile("sgdt %0" : "=m"(currentGdt));

  char expectedLimitStr[11];
  char loadedLimitStr[11];
  char expectedBaseStr[11];
  char loadedBaseStr[11];
  char buffer[256];

  // Convert the expected and loaded limit values to hex strings
  intToHex(gdtDesc.limit, expectedLimitStr);
  intToHex(currentGdt.limit, loadedLimitStr);

  terminalWriteLine("--- GDT Information ---");

  // Print expected and loaded limit values
  terminalWriteLine("Expected Limit:");
  terminalWriteLine(expectedLimitStr);
  terminalWriteLine("Loaded Limit:");
  terminalWriteLine(loadedLimitStr);

  // Convert the expected and loaded base addresses to hex strings
  intToHex((uint32_t)gdt, expectedBaseStr);
  intToHex(currentGdt.base, loadedBaseStr);

  // Print expected and loaded base addresses
  terminalWriteLine("Expected Base:");
  terminalWriteLine(expectedBaseStr);
  terminalWriteLine("Loaded Base:");
  terminalWriteLine(loadedBaseStr);

  // Compare the expected and loaded values
  if (gdtDesc.limit == currentGdt.limit && currentGdt.base == (uint32_t)gdt) {
    terminalWriteLine("GDT loaded correctly!");
  } else {
    terminalWriteLine("GDT load verification failed!");
  }
  
  // Show GDT entries count
  uint32_t entryCount = (currentGdt.limit + 1) / 8;
  char entryCountStr[32];
  uint32ToDecimalString(entryCount, entryCountStr);
  concat("GDT Entries: ", entryCountStr, buffer);
  terminalWriteLine(buffer);
  
  terminalWriteLine("--- End GDT Info ---");
}
