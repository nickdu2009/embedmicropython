# Simple Makefile for MicroPython C++ Integration
BUILD_DIR = external/build
CMAKE = cmake
MAKE = make

# Default target
all: build

# Create build directory and configure
configure:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) ../..

# Build the project
build: configure
	@cd $(BUILD_DIR) && $(MAKE)
	@echo "Build completed successfully!"

# Build in debug mode
debug:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) -DCMAKE_BUILD_TYPE=Debug ../..
	@cd $(BUILD_DIR) && $(MAKE)
	@echo "Debug build completed!"

# Build in release mode
release:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) -DCMAKE_BUILD_TYPE=Release ../..
	@cd $(BUILD_DIR) && $(MAKE)
	@echo "Release build completed!"

# Run basic example
run-basic: build
	@echo "Running basic example..."
	@./$(BUILD_DIR)/basic_example

# Run file example
run-file: build
	@echo "Running file example..."
	@./$(BUILD_DIR)/file_example

# Run script execution example
run-script: build
	@echo "Running script execution example..."
	@./$(BUILD_DIR)/script_execution_example

# Run all examples
run-all: run-basic run-file run-script

# Clean build directory
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Clean completed!"

# Clean all external dependencies and build artifacts
clean-all:
	@rm -rf external/
	@echo "All external dependencies and build artifacts cleaned!"

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
	@echo "  all        - Build the project (default)"
	@echo "  configure  - Configure the project with CMake"
	@echo "  build      - Build the project"
	@echo "  debug      - Build in debug mode"
	@echo "  release    - Build in release mode"
	@echo "  run-basic  - Run basic example"
	@echo "  run-file   - Run file example"
	@echo "  run-script - Run script execution example"
	@echo "  run-all    - Run all examples"
	@echo "  clean      - Clean build directory"
	@echo "  clean-all  - Clean all external dependencies and build artifacts"
	@echo "  install    - Install the library"
	@echo "  package    - Create package"
	@echo "  help       - Show this help"

.PHONY: all configure build debug release run-basic run-file run-all clean clean-all install package help
