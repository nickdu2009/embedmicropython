# Simple Makefile for MicroPython C++ Integration
BUILD_DIR = build
OUTPUT_DIR = output
CMAKE = cmake
MAKE = make

# Default target
all: build

# Create build directory and configure
configure:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) ..

# Build the project
build: configure
	@cd $(BUILD_DIR) && $(MAKE)
	@echo "Build completed successfully!"

# Build in debug mode
debug:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) -DCMAKE_BUILD_TYPE=Debug ..
	@cd $(BUILD_DIR) && $(MAKE)
	@echo "Debug build completed!"

# Build in release mode
release:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) -DCMAKE_BUILD_TYPE=Release ..
	@cd $(BUILD_DIR) && $(MAKE)
	@echo "Release build completed!"

# Run basic example
run-basic: build
	@echo "Running basic example..."
	@./$(OUTPUT_DIR)/bin/basic_example

# Run file example
run-file: build
	@echo "Running file example..."
	@./$(OUTPUT_DIR)/bin/file_example

# Run all examples
run-all: run-basic run-file

# Clean build directory
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Clean completed!"

# Clean output directory
clean-output:
	@rm -rf $(OUTPUT_DIR)/bin $(OUTPUT_DIR)/lib
	@echo "Output directory cleaned!"

# Clean everything
clean-all: clean clean-output
	@echo "All build artifacts cleaned!"

# Install the library
install: build
	@cd $(BUILD_DIR) && $(MAKE) install
	@echo "Install completed!"

# Create package
package: build
	@cd $(BUILD_DIR) && $(MAKE) package
	@echo "Package created!"

# Show help
help:
	@echo "Available targets:"
	@echo "  all          - Build the project (default)"
	@echo "  configure    - Configure the project with CMake"
	@echo "  build        - Build the project"
	@echo "  debug        - Build in debug mode"
	@echo "  release      - Build in release mode"
	@echo "  run-basic    - Run basic example"
	@echo "  run-file     - Run file example"
	@echo "  run-all      - Run all examples"
	@echo "  clean        - Clean build directory"
	@echo "  clean-output - Clean output directory"
	@echo "  clean-all    - Clean everything"
	@echo "  install      - Install the library"
	@echo "  package      - Create package"
	@echo "  help         - Show this help"
	@echo ""
	@echo "Output structure:"
	@echo "  output/bin/  - Executable files"
	@echo "  output/lib/  - Library files"

.PHONY: all configure build debug release run-basic run-file run-all clean clean-output clean-all install package help
