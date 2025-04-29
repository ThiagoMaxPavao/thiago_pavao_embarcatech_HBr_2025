#!/bin/bash

# Get the absolute path to the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Set variables based on script location
BUILD_DIR="$SCRIPT_DIR/test/build"
EXECUTABLE="unit_tests"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Navigate to build directory
cd "$BUILD_DIR"

# Run CMake to configure the build system
cmake ..

# Build the test program
make

# Run the test executable if build was successful
if [ -f "$EXECUTABLE" ]; then
    printf "\nRunning unit tests...\n\n"
    ./"$EXECUTABLE"
else
    echo "Build failed or executable not found: $EXECUTABLE"
    exit 1
fi
