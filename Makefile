# Compiler settings
CXX         := gcc
CXX_FLAGS   := -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LD_FLAGS    := -m32 -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

# Folder & file paths
BIN         := bin
SRC         := src
OBJ         := obj
EXECUTABLE  := myos.bin
BOOT_SRC    := boot.asm
BOOT_OBJ    := $(OBJ)/boot.o

INTERRUPT_SRC := $(SRC)/interrupts_stubs.asm
INTERRUPT_OBJ := $(OBJ)/interrupts_stubs.o

# Source and object files
CFILES      := $(wildcard src/*.c)
OFILES      := $(patsubst src/%.c, obj/%.o, $(CFILES)) $(BOOT_OBJ) $(INTERRUPT_OBJ)

# Default target
all: $(BIN)/$(EXECUTABLE)

# Run the program
run: clean all
	@echo "--------------------------------"
	@echo "Executing..."
	qemu-system-i386 -kernel $(BIN)/$(EXECUTABLE)	-device isa-debug-exit,iobase=0x501,iosize=1

# Clean object and binary files
clean:
	@echo "--------------------------------"
	@echo "Cleaning..."
	-rm -f $(OBJ)/*.o
	-rm -f $(BIN)/$(EXECUTABLE)

# Compile boot.asm to .o file
$(BOOT_OBJ): $(BOOT_SRC)
	@mkdir -p $(OBJ)
	@echo "--------------------------------"
	@echo "Assembling boot.asm"
	nasm -f elf32 $< -o $@

# Assemble interrupts.asm to .o file
$(INTERRUPT_OBJ): $(INTERRUPT_SRC)
	@mkdir -p $(OBJ)
	@echo "--------------------------------"
	@echo "Assembling $<"
	nasm -f elf32 $< -o $@

# Compile each .c into a .o file
$(OBJ)/%.o: $(SRC)/%.c
	@echo "--------------------------------"
	@echo "Compiling $<"
	$(CXX) $(CXX_FLAGS) -c $< -o $@

# Link all .o files
$(BIN)/$(EXECUTABLE): $(OFILES) linker.ld
	@mkdir -p $(BIN)
	@echo "--------------------------------"
	@echo "Linking with GCC and linker.ld into $@"
	$(CXX) -m32 -T linker.ld -ffreestanding -O2 -nostdlib -o $@ $(OFILES) -lgcc
