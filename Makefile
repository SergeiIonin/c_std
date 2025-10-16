# Makefile for C Programming Examples

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -pthread

# Directories
SRC_DIR = src
BIN_DIR = bin

# Ensure bin directory exists
$(shell mkdir -p $(BIN_DIR))

# Get all C source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate binary names from source files
BINS = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(SRCS))

# Default target
all: $(BINS)

# Rule to compile each source file
$(BIN_DIR)/%: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
	@echo "Compiled: $@"

# Individual targets for each example
hello_world: $(BIN_DIR)/hello_world
simple_threading: $(BIN_DIR)/simple_threading
pointer_examples: $(BIN_DIR)/pointer_examples

# Run targets
run_hello_world: hello_world
	@echo "Running hello_world:"
	@$(BIN_DIR)/hello_world

run_simple_threading: simple_threading
	@echo "Running simple_threading:"
	@$(BIN_DIR)/simple_threading

run_pointer_examples: pointer_examples
	@echo "Running pointer_examples:"
	@$(BIN_DIR)/pointer_examples

# Clean compiled files
clean:
	@rm -rf $(BIN_DIR)
	@echo "Cleaned compiled files"

# Help target
help:
	@echo "Available targets:"
	@echo "  all                 - Compile all examples"
	@echo "  clean               - Remove all compiled files"
	@echo "  hello_world         - Compile hello_world example"
	@echo "  run_hello_world     - Run hello_world example"
	@echo "  simple_threading    - Compile simple_threading example"
	@echo "  run_simple_threading - Run simple_threading example"
	@echo "  pointer_examples    - Compile pointer_examples example"
	@echo "  run_pointer_examples - Run pointer_examples example"

.PHONY: all clean help hello_world run_hello_world simple_threading run_simple_threading pointer_examples run_pointer_examples
