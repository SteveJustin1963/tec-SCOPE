# ============================================================================
# Makefile for Telescope Stepper Motor Control
# ============================================================================

# Assembler selection (choose one)
# ASM = z80asm
ASM = tasm
# ASM = sjasm

# Assembler flags
ASMFLAGS_Z80ASM = -o
ASMFLAGS_TASM = -80 -b
ASMFLAGS_SJASM = --raw

# Output files
MAIN_BIN = telescope.bin
SIMPLE_BIN = simple-test.bin

# Source files
MAIN_SRC = main.z80
SIMPLE_SRC = simple-test.z80
LIB_SRC = stepper-lib.z80
KEYPAD_SRC = keypad-input.z80
CONFIG_SRC = config.z80

# ============================================================================
# Build targets
# ============================================================================

.PHONY: all clean help main simple

all: main simple

# Build main telescope control program
main: $(MAIN_BIN)

$(MAIN_BIN): $(MAIN_SRC) $(LIB_SRC) $(KEYPAD_SRC) $(CONFIG_SRC)
	@echo "Assembling main program..."
ifeq ($(ASM),tasm)
	$(ASM) $(ASMFLAGS_TASM) $(MAIN_SRC) $(MAIN_BIN)
else ifeq ($(ASM),z80asm)
	$(ASM) $(ASMFLAGS_Z80ASM) $(MAIN_BIN) $(MAIN_SRC)
else
	$(ASM) $(ASMFLAGS_SJASM) $(MAIN_SRC) $(MAIN_BIN)
endif
	@echo "Main program built: $(MAIN_BIN)"

# Build simple test program
simple: $(SIMPLE_BIN)

$(SIMPLE_BIN): $(SIMPLE_SRC)
	@echo "Assembling simple test program..."
ifeq ($(ASM),tasm)
	$(ASM) $(ASMFLAGS_TASM) $(SIMPLE_SRC) $(SIMPLE_BIN)
else ifeq ($(ASM),z80asm)
	$(ASM) $(ASMFLAGS_Z80ASM) $(SIMPLE_BIN) $(SIMPLE_SRC)
else
	$(ASM) $(ASMFLAGS_SJASM) $(SIMPLE_SRC) $(SIMPLE_BIN)
endif
	@echo "Simple test built: $(SIMPLE_BIN)"

# Clean build artifacts
clean:
	@echo "Cleaning build files..."
	rm -f *.bin *.lst *.hex *.obj
	@echo "Clean complete"

# Create Intel HEX format (if needed)
hex: $(MAIN_BIN)
	@echo "Converting to Intel HEX format..."
	objcopy -I binary -O ihex $(MAIN_BIN) telescope.hex
	@echo "HEX file created: telescope.hex"

# Help
help:
	@echo "Telescope Stepper Motor Control - Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build both main and simple test programs (default)"
	@echo "  main     - Build main telescope control program"
	@echo "  simple   - Build simple test program"
	@echo "  hex      - Convert main program to Intel HEX format"
	@echo "  clean    - Remove all build artifacts"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Assembler Options:"
	@echo "  Edit this Makefile to select your assembler:"
	@echo "    ASM = tasm    (Table Assembler)"
	@echo "    ASM = z80asm  (Z80 Assembler)"
	@echo "    ASM = sjasm   (SjASM)"
	@echo ""
	@echo "Files:"
	@echo "  main.z80        - Main control program with keypad interface"
	@echo "  simple-test.z80 - Simple test program"
	@echo "  stepper-lib.z80 - Stepper motor library"
	@echo "  keypad-input.z80 - TEC-1 keypad interface (NMI-based)"
	@echo "  config.z80      - Configuration and constants"

# ============================================================================
