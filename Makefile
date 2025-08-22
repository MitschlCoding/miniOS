# Compiler settings
CXX         := gcc
CXX_FLAGS   := -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LD_FLAGS    := -m32 -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

# Folder & file paths
BIN         := bin
SRC         := src
OBJ         := obj
EXECUTABLE  := miniOS.bin
BOOT_SRC    := boot.asm
BOOT_OBJ    := $(OBJ)/boot.o

INTERRUPT_SRC := $(SRC)/interrupts_stubs.asm
INTERRUPT_OBJ := $(OBJ)/interrupts_stubs.o

# Source and object files
CFILES      := $(wildcard src/*.c)
OFILES      := $(patsubst src/%.c, obj/%.o, $(CFILES)) $(BOOT_OBJ) $(INTERRUPT_OBJ)

# Default target
all: $(BIN)/$(EXECUTABLE)

# Target to install dependencies
installDeps:
	@echo "--------------------------------"
	@echo "Installing dependencies..."
	sudo apt-get update
	sudo apt-get install -y qemu-system nasm build-essential libtool gcc-multilib libc6-dev-i386
	@echo "Installation finished."
	@echo "--------------------------------"

installBuildDeps:
	@echo "--------------------------------"
	@echo "Installing dependencies..."
	sudo apt-get update
	sudo apt-get install -y nasm build-essential libtool gcc-multilib libc6-dev-i386
	@echo "Installation finished."
	@echo "--------------------------------"

# Generate documentation with Doxygen
docs:
	@echo "--------------------------------"
	@echo "Generating Doxygen documentation..."
	@if [ ! -f Doxyfile ]; then \
		echo "Creating Doxyfile..."; \
		doxygen -g; \
		sed -i 's/PROJECT_NAME           = "My Project"/PROJECT_NAME           = "miniOS"/' Doxyfile; \
		sed -i 's/INPUT                  =/INPUT                  = src/' Doxyfile; \
		sed -i 's/OUTPUT_DIRECTORY       =/OUTPUT_DIRECTORY       = docs/' Doxyfile; \
		sed -i 's/GENERATE_LATEX         = YES/GENERATE_LATEX         = NO/' Doxyfile; \
		sed -i 's/RECURSIVE              = NO/RECURSIVE              = YES/' Doxyfile; \
		sed -i 's/EXTRACT_ALL            = NO/EXTRACT_ALL            = YES/' Doxyfile; \
		sed -i 's/EXTRACT_PRIVATE        = NO/EXTRACT_PRIVATE        = YES/' Doxyfile; \
		sed -i 's/EXTRACT_STATIC         = NO/EXTRACT_STATIC         = YES/' Doxyfile; \
	fi
	@mkdir -p docs
	doxygen Doxyfile
	@echo "Documentation generated in docs/html/"
	@echo "Open docs/html/index.html in your browser to view the documentation."

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

# Clean everything including documentation
cleanall: clean
	@echo "Cleaning documentation..."
	-rm -rf docs/
	-rm -f Doxyfile

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


