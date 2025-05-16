#include "idt.h"
#include "outb.h"
#include "printOS.h"
#include <stdint.h>

// the idt with 256 entries
IdtEntry idtEntries[256];

// the descriptor of our idt
IdtDescriptor idtDescriptor;

// These are all the stubs generated in our interrupts_stubs.asm file
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr33();

// a list of all the cpu interrupts. interrupt numbers: 0-31
void *isrStubTable[32] = {
    isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7,  isr8,  isr9,  isr10,
    isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr20, isr21,
    isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31};

// a function to return the interrupt with number i
uint32_t getStubAddr(int i) { return (uint32_t)isrStubTable[i]; }

// initialize the programmable interrupt controller
void pic_init() {
  // send bytes to PICs Command-Ports to start configuration
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  ioWait();
  // send bytes to PICs Data_ports to set the offset for our interrupt-Numbers
  // so they dont overlap with our cpu interrupts (new interrupts are from 32
  // onward) send bytes for offset to not overlap with cpu interrupts, to the
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  ioWait();
  // configure Cascading
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  ioWait();
  // set mode
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  ioWait();

  // mask all interrupts except 33 for keyboard
  outb(0x21, 0xFD);
  outb(0xA1, 0xFF);
  ioWait();
}

void idtInit() {
  // setup our pic
  pic_init();

  // for each cpu interrupt 0-31 map it to our stub, that then calls our
  // isrHandler function
  for (int i = 0; i < 32; i++) {
    uint32_t addr = getStubAddr(i);
    uint16_t lowerBase = addr & 0xFFFF;
    uint16_t higherBase = (addr >> 16) & 0xFFFF;
    idtEntries[i].lowerBase = lowerBase;
    idtEntries[i].higherBase = higherBase;
    idtEntries[i].kernelCodeSegment = 0x08;
    idtEntries[i].zero = 0;
    idtEntries[i].typeAttribute = 0x8E;
  }
  // map our isr33 keyboard interrupt to use the stub, that then calls our
  // isrHandler function
  uint32_t addr = (uint32_t)isr33;
  uint16_t lowerBase = addr & 0xFFFF;
  uint16_t higherBase = (addr >> 16) & 0xFFFF;
  idtEntries[33].lowerBase = lowerBase;
  idtEntries[33].higherBase = higherBase;
  idtEntries[33].kernelCodeSegment = 0x08;
  idtEntries[33].zero = 0;
  idtEntries[33].typeAttribute = 0x8E;

  // generate the idtDescriptor
  idtDescriptor.base = (uint32_t)&idtEntries;
  idtDescriptor.limit = (sizeof(IdtEntry) * 256) - 1;

  // set idt descriptor to be used by cpu
  __asm__ volatile("lidt (%0)" : : "r"(&idtDescriptor) : "memory");
}

// Prints the currently loaded IDT information read from the CPU
void printIdtInfo() {
  // Struct to load current IDT info into
  IdtDescriptor currentIdt;

  // Read the current IDT descriptor using sidt instruction
  __asm__ volatile("sidt %0" : "=m"(currentIdt));

  char loadedLimitStr[11]; // Buffer for hex string (uint16_t needs less, but
                           // reuse size)
  char loadedBaseStr[11];  // Buffer for hex string (uint32_t)
  int line = 0;

  terminalWriteLine("--- Current IDT Information (from CPU) ---", line++);

  // Convert the loaded limit value to hex string
  // Note: IDT limit is only 16 bits
  intToHex((uint32_t)currentIdt.limit,
           loadedLimitStr); // Cast for intToHex if needed

  // Print loaded limit value
  terminalWriteLine("Loaded Limit:", line++);
  terminalWriteLine(loadedLimitStr, line++);

  // Convert the loaded base address to hex string
  intToHex(currentIdt.base, loadedBaseStr);

  // Print loaded base address
  terminalWriteLine("Loaded Base:", line++);
  terminalWriteLine(loadedBaseStr, line++);

  terminalWriteLine("--- End IDT Info ---", line++);
  terminalWriteLine("Press enter to continue...", line + 1);
}
