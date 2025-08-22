# miniOS Documentation {#mainpage}

## Overview

Welcome to the **miniOS** documentation! This is a custom operating system kernel written in C and assembly, designed as a learning project to understand low-level system programming and operating system concepts.

## Features

-   **Custom Bootloader**: Assembly-based bootloader for kernel initialization
-   **Mode**: 32-bit protected mode operation
-   **Memory Address Space**: 4GB linear address space implementing a GDT
-   **Memory Management**: Custom heap implementation and memory allocation
-   **Interrupt Handling**: Complete IDT (Interrupt Descriptor Table) setup and interrupt management
-   **Keyboard Input**: PS/2 keyboard driver for user input
-   **Terminal Interface**: Basic terminal with command processing
-   **Graphics**: VGA text mode output with custom print functions
-   **Game Implementation**: Snake game as an interactive application
-   **Time Management**: System timer and time-based functions

## Architecture

### Core Components

-   **Kernel (`kernel.c`)**: Main entry point and system initialization
-   **GDT (`gdt.c/.h`)**: Global Descriptor Table setup for memory segmentation
-   **IDT (`idt.c/.h`)**: Interrupt Descriptor Table for interrupt handling
-   **Memory (`heap.c/.h`)**: Dynamic memory allocation system
-   **Terminal (`terminal.c/.h`)**: Text-based user interface
-   **Keyboard (`keyboard.c/.h`)**: Input device driver

### System Services

-   **Command Handler (`commandHandler.c/.h`)**: Command-line interface processing
-   **Print System (`printOS.c/.h`)**: Formatted output and display functions
-   **String Utilities (`str.c/.h`)**: String manipulation functions
-   **Time Services (`time.c/.h`)**: System timing and delays

### Applications

-   **Snake Game (`snake.c/.h`)**: Interactive snake game implementation
-   **Mode Manager (`modeManager.c/.h`)**: Application mode switching. From Terminal to Visual Applications.

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

# Build the kernel
make

# Run in QEMU
make run

# Generate documentation (see our github pages for generated docs)
make docs
```

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

This project is developed for educational purposes.

---

_Generated with Doxygen - Last updated: 2025_
