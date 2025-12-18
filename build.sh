#!/bin/bash

echo "Building coreMusicPlayer..."
mkdir -p build
cd build
cmake ..
if [ $? -eq 0 ]; then
    echo "CMake configuration successful"
    make
    if [ $? -eq 0 ]; then
        echo "Build completed successfully!"
        echo "Executable is in build/bin/core_music_player"
    else
        echo "Build failed!"
    fi
else
    echo "CMake configuration failed!"
fi
cd ..