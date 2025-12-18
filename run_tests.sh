#!/bin/bash

# Run tests script for Linux/macOS

echo "Running coreMusicPlayer tests..."

# Change to build directory
cd build

# Run the test executable
./coreMusicPlayerTests

if [ $? -ne 0 ]; then
    echo "Tests failed!"
    exit 1
fi

echo "All tests passed!"