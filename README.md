# miniOS

A minimal 32-bit operating system written from scratch in C and Assembly, featuring a custom kernel, interrupt handling, memory management, and an interactive terminal with built-in applications and PC Speaker audio support.

## Quick Start

```bash
# Install dependencies (Ubuntu)
make installDeps

# Build and run with audio support (default)
make

# Build only (no execution)
make build

# Run with audio support (requires prior build)
make run

# Run without audio support (requires prior build)
make runNoAudio
```

**What to try:**

1. Press ENTER through the system info screens to reach the terminal
2. Type `help` to see available commands
3. Type `snake` to play the built-in Snake game (WASD to move, ESC to quit)
4. Type `beep` to test the PC Speaker audio system
5. Type `music` to play musical melodies
6. Type `shutdown` to exit

## Audio Support

miniOS includes PC Speaker audio support for basic sound effects and beeps. Audio is enabled by default:

-   **Default behavior**: `make` or `make run` (includes audio)
-   **Without audio**: `make runNoAudio` (disables PC Speaker)

**Audio Requirements:**

-   PulseAudio installed and running on Ubuntu
-   Audio system properly configured
-   QEMU with audio device support

The audio system provides simple beep commands and can be extended for more complex sound effects.

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
-   **PC Speaker Audio**: Hardware-based sound generation using PIT Channel 2

### Available Commands

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

To build and run the OS with audio support (default behavior):

```bash
make
```

To build the OS without running it:

```bash
make build
```

To run the OS with audio support (requires prior build):

```bash
make run
```

To run the OS without audio support (requires prior build):

```bash
make runNoAudio
```

**Note for Audio**: Audio is enabled by default and uses PC Speaker emulation in QEMU. This requires:

-   PulseAudio running on your Ubuntu system
-   Proper audio configuration
-   The `beep` and `music` commands will produce actual sounds when audio is enabled

To clean all object-files and bins use:

```bash
make clean
```

## License

This project is under the GNU GENERAL PUBLIC LICENSE Version 3
