#!/bin/bash

# MicroPython C++ Integration - Dependencies Setup Script
# This script downloads and configures MicroPython for embedding

set -e  # Exit on any error

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
EXTERNAL_DIR="$PROJECT_ROOT/external"

echo "=== MicroPython C++ Integration Dependencies Setup ==="
echo "Project root: $PROJECT_ROOT"
echo "External directory: $EXTERNAL_DIR"

# Create external directory if it doesn't exist
mkdir -p "$EXTERNAL_DIR"
cd "$EXTERNAL_DIR"

# Download MicroPython source if not exists
if [ ! -d "micropython" ]; then
    echo "Downloading MicroPython source..."
    git clone https://github.com/micropython/micropython.git
    echo "MicroPython source downloaded successfully!"
else
    echo "MicroPython source already exists, skipping download."
fi

# Build MicroPython embed library
echo "Building MicroPython embed library..."
cd micropython/examples/embedding

if [ ! -d "micropython_embed" ]; then
    echo "Building embed library..."
    make -f micropython_embed.mk
    echo "MicroPython embed library built successfully!"
else
    echo "MicroPython embed library already exists."
fi

# Copy embed library to external directory
echo "Copying embed library to external directory..."
cp -r micropython_embed "$EXTERNAL_DIR/"
echo "Embed library copied successfully!"

cd "$PROJECT_ROOT"

echo ""
echo "=== Dependencies Setup Complete ==="
echo "You can now build the project with:"
echo "  make build                     # Build with stub implementation"
echo "  make clean && cmake -DUSE_REAL_MICROPYTHON=ON -B external/build && make -C external/build"
echo "                                 # Build with real MicroPython"
echo ""
echo "External directory structure:"
echo "  external/"
echo "    ├── build/                   # Build artifacts"
echo "    ├── micropython/             # MicroPython source code"
echo "    └── micropython_embed/       # MicroPython embed library"
