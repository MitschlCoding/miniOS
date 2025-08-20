# miniOS

A minimal 32-bit operating system written from scratch in C and Assembly, featuring a custom kernel, interrupt handling, memory management, and an interactive terminal with built-in applications.

## Quick Start

```bash
# Install dependencies (Ubuntu)
make installDeps

# Build and run
make run
```

**What to try:**

1. Press ENTER through the system info screens to reach the terminal
2. Type `help` to see available commands
3. Type `snake` to play the built-in Snake game (WASD to move, ESC to quit)
4. Type `shutdown` to exit

## Overview

miniOS is a educational operating system project that demonstrates fundamental OS concepts including:

-   **Custom Bootloader**: GRUB-compatible multiboot kernel
-   **Hardware Abstraction**: VGA text mode display, keyboard input, timer interrupts
-   **Memory Management**: Custom heap implementation with dynamic allocation
-   **Interrupt Handling**: Complete IDT setup with exception and hardware interrupt support
-   **Modular Architecture**: Clean separation between kernel, drivers, and applications

## Features

### Core System

-   **32-bit x86 Kernel**: Written in C with Assembly bootstrap
-   **Global Descriptor Table (GDT)**: Memory segmentation setup
-   **Interrupt Descriptor Table (IDT)**: Exception and interrupt handling
-   **PIC Configuration**: Programmable Interrupt Controller setup
-   **VGA Text Mode**: 80x25 character display with color support

### Interactive Applications

-   **Terminal Interface**: Command-line interface with command history
-   **Snake Game**: Classic snake game with WASD controls
-   **Mode Manager**: Flexible system for switching between terminal and visual modes

### Memory & I/O

-   **Heap Management**: Custom `mallocOS()` and `freeOS()` implementation
-   **Keyboard Driver**: PS/2 keyboard support with scan code translation
-   **Timer System**: PIT-based timing for game logic and system events
-   **Screen Management**: Direct VGA buffer manipulation for fast rendering

### Available Commands

-   `help` - Display available commands and usage information
-   `shutdown` - Gracefully shutdown the system (QEMU)
-   `snake` - Launch the built-in Snake game

### Technical Highlights

-   **Exception Handling**: Divide by zero, page faults, general protection faults
-   **Hardware Interrupts**: Keyboard (IRQ1), Timer (IRQ0)
-   **Function Pointers**: Extensible visual mode system for easy game/app integration
-   **String Library**: Custom implementations of strlen, strcmp, split, and more
-   **Multiboot Support**: GRUB-compatible bootloader interface

## Getting Started

### First Boot Experience

1. **System Information**: View multiboot info, memory map, GDT, and IDT details
2. **Terminal Access**: Press ENTER through the info screens to reach the terminal
3. **Try Commands**: Type `help` to see available commands
4. **Play Snake**: Type `snake` to launch the game (use WASD to control, ESC to quit)

## Dependencies

-   qemu-system
-   nasm
-   build-essential
-   libtool
-   gcc-multilib
-   libc6-dev-i386

## Commands

If you have make installed and use Ubuntu, you can use the following command to install the dependencies:

```bash
make installDeps
```

To build and run the OS using qemu use:

```bash
make run
```

To clean all object-files and bins use:

```bash
make clean
```
