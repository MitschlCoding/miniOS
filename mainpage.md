# miniOS Documentation {#mainpage}

## Overview

Welcome to the **miniOS** documentation! This is a custom operating system kernel written in C and assembly, designed as a learning project to understand low-level system programming and operating system concepts.

## Features

-   **GRUB-Compatible**: Assembly-based bootloader for kernel initialization
-   **Mode**: 32-bit protected mode operation
-   **Memory Address Space**: 4GB linear address space implementing a GDT
-   **Memory Management**: Custom heap implementation and memory allocation
-   **Interrupt Handling**: Complete IDT (Interrupt Descriptor Table) setup and interrupt management
-   **Keyboard Input**: PS/2 keyboard driver for user input
-   **Terminal Interface**: Basic terminal with command processing
-   **Graphics**: VGA text mode output with custom print functions
-   **Game Implementation**: Snake game as an interactive application
-   **Time Management**: System timer and time-based functions
-   **PC Speaker Audio**: Hardware-based sound generation using PIT Channel 2

## Architecture

### Core Components

-   **Kernel** (`kernel.c`): Main entry point and system initialization
-   **GDT** (`gdt.h`/`gdt.c`): Global Descriptor Table setup for memory segmentation
-   **IDT** (`idt.h`/`idt.c`): Interrupt Descriptor Table for interrupt handling
-   **Memory** (`heap.h`/`heap.c`): Dynamic memory allocation system
-   **Terminal** (`terminal.h`/`terminal.c`): Text-based user interface
-   **Keyboard** (`keyboard.h`/`keyboard.c`): Input device driver

### System Services

-   **Command Handler** (`commandHandler.h`/`commandHandler.c`): Command-line interface processing
-   **Print System** (`printOS.h`/`printOS.c`): Formatted output and display functions
-   **String Utilities** (`str.h`/`str.c`): String manipulation functions
-   **Time Services** (`time.h`/`time.c`): System timing and delays
-   **I/O Operations** (`io.h`/`io.c`): Hardware port input/output functions
-   **Audio System** (`audio.h`/`audio.c`): PC Speaker sound generation

### Applications

-   **Snake Game** (`snake.h`/`snake.c`): Interactive snake game implementation
-   **Mode Manager** (`modeManager.h`/`modeManager.c`): Application mode switching. From Terminal to Visual Applications.

## Getting Started

### Prerequisites

-   GCC with 32-bit support
-   NASM assembler
-   QEMU for emulation
-   Make build system

### Building

```bash
# Install dependencies for Ubuntu
make installDeps

# Build and run with audio support (default behavior)
make

# Build only (no execution)
make build

# Run with audio support (build first if not using the provided binary)
make run

# Run without audio support (build first if not using the provided binary)
make runNoAudio

# Generate documentation (see our github pages for generated docs)
make docs
```

### Audio Support

Audio is enabled by default when building and running miniOS. The system uses PC Speaker emulation in QEMU for audio feedback. Requirements:

-   **PulseAudio**: Must be installed and running on Ubuntu
-   **Audio Configuration**: System audio properly configured
-   **QEMU Audio**: Uses PulseAudio backend for PC Speaker emulation

To run without audio, use `make runNoAudio`.

Available audio commands:

-   `beep` - Test PC Speaker with system beep
-   `music` - Play musical melodies using the PC speaker

### Terminal Commands

The miniOS terminal provides various system commands:

-   `help` - Display available commands and usage information
-   `shutdown` - Gracefully shutdown the system (QEMU)
-   `snake` - Launch the built-in Snake game
-   `uptime` - Show system uptime since boot
-   `gdt` - Show Global Descriptor Table information
-   `idt` - Display Interrupt Descriptor Table details
-   `sysinfo` - Display comprehensive system information
-   `memory` - Display memory information and statistics
-   `beep` - Test the PC Speaker audio system
-   `music` - Play musical melodies using the PC speaker

### Project Structure

```
miniOS/
├── src/           # Source code files
├── obj/           # Compiled object files
├── bin/           # Final kernel binary
├── docs/          # Generated documentation
├── boot.asm       # Bootloader assembly code
├── linker.ld      # Linker script
└── Makefile       # Build configuration
```

## Development

This project follows standard C99 conventions.

### Key Design Principles

-   **Modular Architecture**: Each subsystem is implemented as separate modules
-   **Clear Interfaces**: Well-defined APIs between components
-   **Hardware Abstraction**: Direct hardware interaction through controlled interfaces

## License

This project is under the GNU GENERAL PUBLIC LICENSE Version 3

---

_Generated with Doxygen - Last updated: 2025_
