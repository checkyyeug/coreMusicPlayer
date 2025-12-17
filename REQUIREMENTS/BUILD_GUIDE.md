# coreMusicPlayer Build Guide

## 1. Prerequisites

### 1.1 System Requirements

#### Windows
- Windows 10/11 (64-bit)
- Visual Studio 2019 or 2022 with C++ support
- Windows SDK 10.0 or later
- At least 4GB RAM
- 2GB free disk space

#### Linux
- Ubuntu 20.04+ / Fedora 32+ / openSUSE 15+
- GCC 9+ or Clang 10+
- ALSA development libraries
- At least 4GB RAM
- 2GB free disk space

### 1.2 Required Tools

#### Windows
```bash
# Install Visual Studio Build Tools
# Download from: https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022
# Select "C++ build tools" with Windows 10/11 SDK

# Install Git
# Download from: https://git-scm.com/download/win
```

#### Linux
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install -y build-essential cmake git libasound2-dev

# Fedora
sudo dnf install -y gcc-c++ cmake git alsa-lib-devel

# openSUSE
sudo zypper install -y gcc-c++ cmake git libasound2-devel
```

## 2. Build Configuration Management

### 2.1 CMake Configuration Templates

#### Main CMakeLists.txt Structure
```cmake
cmake_minimum_required(VERSION 3.10)
project(coreMusicPlayer VERSION 2.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Platform detection
if(WIN32)
    set(MP_PLATFORM_WINDOWS TRUE)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(APPLE)
    set(MP_PLATFORM_MACOS TRUE)
    add_definitions(-DPLATFORM_MACOS)
elseif(UNIX)
    set(MP_PLATFORM_LINUX TRUE)
    add_definitions(-DPLATFORM_LINUX)
endif()

# Compiler-specific settings
if(MSVC)
    add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# SIMD optimization flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /arch:AVX2")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles("
        #include <immintrin.h>
        int main() { __m256d x = _mm256_set1_pd(1.0); return 0; }
    " HAVE_AVX2)
    if(HAVE_AVX2)
        add_definitions(-DHAVE_AVX2)
    endif()
endif()

# Find required packages
find_package(PkgConfig QUIET)

# Audio libraries
if(WIN32)
    # Windows comes with WASAPI
    set(PLATFORM_LIBS ole32 uuid winmm)
elseif(UNIX AND NOT APPLE)
    pkg_check_modules(ALSA REQUIRED alsa)
    set(PLATFORM_LIBS ${ALSA_LIBRARIES})
    include_directories(${ALSA_INCLUDE_DIRS})
elseif(APPLE)
    set(PLATFORM_LIBS "-framework CoreAudio -framework AudioToolbox")
endif()

# Find Qt6 (optional, for GUI)
option(BUILD_GUI "Build Qt6 GUI" ON)
if(BUILD_GUI)
    find_package(Qt6 COMPONENTS Core Widgets Multimedia QUIET)
    if(Qt6_FOUND)
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTORCC ON)
        set(CMAKE_AUTOUIC ON)
        add_definitions(-DWITH_QT_GUI)
    else()
        message(WARNING "Qt6 not found, GUI will not be built")
        set(BUILD_GUI OFF)
    endif()
endif()

# Include subdirectories
add_subdirectory(src)

# Testing
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Documentation
option(BUILD_DOCS "Build documentation" OFF)
if(BUILD_DOCS)
    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        add_subdirectory(docs)
    endif()
endif()
```

### 2.2 Dependency Management with vcpkg

#### vcpkg.json for Windows
```json
{
  "name": "coremusicplayer",
  "version": "2.0.0",
  "description": "Cross-platform audio player",
  "dependencies": [
    {
      "name": "qtbase",
      "version>=": "6.5.0",
      "features": ["widgets", "multimedia"]
    },
    {
      "name": "qttools",
      "version>=": "6.5.0",
      "features": ["linguist"]
    },
    {
      "name": "ffmpeg",
      "version>=": "5.0",
      "features": ["avcodec", "avformat", "swresample"]
    },
    {
      "name": "nlohmann-json",
      "version>=": "3.11.0"
    },
    {
      "name": "gtest",
      "version>=": "1.12.0"
    }
  ],
  "builtin-baseline": "2023-04-15"
}
```

### 2.3 Platform-Specific Scripts

#### Windows Build Scripts
```batch
@echo off
REM build_windows.bat - Main Windows build script
echo Building coreMusicPlayer for Windows...

REM Set environment variables
set QT_DIR=C:\Qt\6.10.1\mingw_64
set MINGW_DIR=C:\Qt\Tools\mingw1310_64
set PATH=%QT_DIR%\bin;%MINGW_DIR%\bin;%PATH%

REM Check if Qt is installed
if not exist "%QT_DIR%" (
    echo Qt not found at %QT_DIR%
    echo Please install Qt first
    exit /b 1
)

REM Create build directory
if not exist build_windows mkdir build_windows
cd build_windows

REM Configure with CMake
cmake .. -G "MinGW Makefiles" ^
    -DCMAKE_PREFIX_PATH="%QT_DIR%" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX=../install

REM Build the project
cmake --build . --config Release

REM Run tests
if exist tests\run_tests.bat (
    echo Running tests...
    tests\run_tests.bat
)

echo Build completed successfully!
```

#### Linux Build Scripts
```bash
#!/bin/bash
# build_linux.sh - Main Linux build script
echo "Building coreMusicPlayer for Linux..."

# Check dependencies
check_dependencies() {
    local deps=("cmake" "g++" "make" "pkg-config")
    for dep in "${deps[@]}"; do
        if ! command -v $dep &> /dev/null; then
            echo "Error: $dep is not installed"
            echo "Please install: sudo apt install $dep"
            exit 1
        fi
    done

    # Check for ALSA development libraries
    if ! pkg-config --exists alsa; then
        echo "Error: ALSA development libraries not found"
        echo "Please install: sudo apt install libasound2-dev"
        exit 1
    fi
}

# Set build type
BUILD_TYPE=${1:-Release}

# Create build directory
mkdir -p build_linux
cd build_linux

# Configure with CMake
cmake .. \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_C_COMPILER=gcc

# Build the project
make -j$(nproc)

# Install (optional)
if [ "$2" = "install" ]; then
    sudo make install
fi

# Create package
make package

echo "Build completed successfully!"
```

#### Cross-Platform Build Script
```batch
@echo off
REM build_cross_platform.bat - Cross-platform build orchestrator
echo Building coreMusicPlayer for all platforms...

REM Detect platform
if "%OS%"=="Windows_NT" (
    echo Detected Windows platform
    call build_windows.bat
) else if "%OS%"=="Linux" (
    echo Detected Linux platform
    bash build_linux.sh
) else if "%OS%"=="Darwin" (
    echo Detected macOS platform
    bash build_macos.sh
) else (
    echo Unknown platform: %OS%
    exit /b 1
)

echo Cross-platform build completed!
```

#### CI/CD Build Scripts
```yaml
# .github/workflows/build.yml
name: Build coreMusicPlayer

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  windows:
    runs-on: windows-latest
    strategy:
      matrix:
        build_type: [Debug, Release]

    steps:
    - uses: actions/checkout@v3

    - name: Install vcpkg
      run: |
        git clone https://github.com/Microsoft/vcpkg.git
        ./vcpkg/bootstrap-vcpkg.bat
        ./vcpkg/vcpkg integrate install

    - name: Configure CMake
      run: |
        cmake -B build_x64 ^
          -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
          -DVCPKG_TARGET_TRIPLET=x64-windows ^
          -DCMAKE_BUILD_TYPE=${{matrix.build_type}}

    - name: Build
      run: cmake --build build_x64 --config ${{matrix.build_type}}

    - name: Test
      run: ctest -C build_x64 --config ${{matrix.build_type}}

  linux:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        build_type: [Debug, Release]

    steps:
    - uses: actions/checkout@v3

    - name: Install dependencies
      run: |
        sudo apt update
        sudo apt install -y cmake build-essential libasound2-dev qt6-base-dev qt6-multimedia-dev

    - name: Configure CMake
      run: |
        cmake -B build -DCMAKE_BUILD_TYPE=${{matrix.build_type}}

    - name: Build
      run: cmake --build build --config ${{matrix.build_type}}

    - name: Test
      run: ctest --test-dir build
```

## 3. Build Configuration Management

### 3.1 Build Options

#### Debug Build
```bash
# Enable debug symbols and disable optimizations
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DENABLE_PROFILING=ON \
      -DENABLE_LOGGING=ON \
      -DBUILD_TESTS=ON
```

#### Release Build
```bash
# Optimize for performance
cmake -DCMAKE_BUILD_TYPE=Release \
      -DENABLE_SIMD=ON \
      -DSTRIP_SYMBOLS=ON \
      -DBUILD_TESTS=OFF
```

### 3.2 Optional Features

#### Feature Flags
```cmake
# Audio format support
option(ENABLE_MP3 "Enable MP3 support" ON)
option(ENABLE_FLAC "Enable FLAC support" ON)
option(ENABLE_OGG "Enable OGG Vorbis support" ON)
option(ENABLE_WAVPACK "Enable WavPack support" OFF)

# GUI features
option(ENABLE_VISUALIZATIONS "Enable audio visualizations" OFF)
option(ENABLE_SKINS "Enable custom skin support" OFF)
option(ENABLE_PLUGINS "Enable plugin system" ON)

# Performance features
option(ENABLE_SIMD "Enable SIMD optimizations" ON)
option(ENABLE_MULTITHREADING "Enable multi-threading" ON)
option(ENABLE_MEMORY_POOL "Enable memory pooling" ON)
```

#### Conditional Compilation
```cpp
#ifdef ENABLE_MP3
#include "mp3_decoder.h"
#endif

#ifdef ENABLE_SIMD
    #ifdef HAVE_AVX2
        // Use AVX2 optimized code
        process_audio_avx2(buffer);
    #elif defined(HAVE_SSE2)
        // Use SSE2 optimized code
        process_audio_sse2(buffer);
    #else
        // Use scalar code
        process_audio_scalar(buffer);
    #endif
#endif
```

## 4. Building Qt GUI Application

### 4.1 Qt Requirements
```

#### macOS Build Scripts (Future)
```bash
#!/bin/bash
# build_macos.sh - macOS build script
echo "Building coreMusicPlayer for macOS..."

# Check dependencies
if ! command -v cmake &> /dev/null; then
    echo "Error: cmake is not installed"
    echo "Please install: brew install cmake"
    exit 1
fi

if ! command -v xcodebuild &> /dev/null; then
    echo "Error: Xcode command line tools not installed"
    echo "Please install: xcode-select --install"
    exit 1
fi

# Set build type
BUILD_TYPE=${1:-Release}

# Create build directory
mkdir -p build_macos
cd build_macos

# Configure with CMake
cmake .. \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"

# Build the project
make -j$(sysctl -n hw.ncpu)

# Create application bundle
make package

echo "Build completed successfully!"
```

## 3. Building Qt GUI Application

### 3.1 Qt Requirements

#### Windows
```bash
# Option 1: Qt Online Installer (Recommended)
# Download from: https://www.qt.io/download-qt-installer
# Install:
# - Qt 6.10.1
# - MinGW 13.1.0 (if not using MSVC)
# - Qt Creator (optional)

# Option 2: Pre-built Binaries
# Download Qt 6.10.1 MinGW from: https://download.qt.io/archive/qt/
# Extract to C:\Qt\6.10.1\mingw_64
```

#### Linux
```bash
# Ubuntu/Debian
sudo apt install -y qt6-base-dev qt6-tools-dev qt6-multimedia-dev

# Fedora
sudo dnf install -y qt6-qtbase-devel qt6-qttools-devel qt6-qtmultimedia-devel

# Arch Linux
sudo pacman -S qt6-base qt6-tools qt6-multimedia
```

### 2.2 Build Commands

#### Windows (Qt GUI)
```cmd
cd qt_gui

# Build the application
build.bat

# Deploy with all required DLLs
build.bat deploy

# Run the application
deploy\QtGuiPlayer.exe
```

#### Linux (Qt GUI)
```bash
cd qt_gui

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
make -j4

# Install (optional)
sudo make install
```

## 3. Building Console Applications

### 3.1 Windows

#### Using build_final.bat (Recommended)
```cmd
# Main build script
build_final.bat

# Output in: build_final\Release\
```

#### Manual Build
```cmd
# Create build directory
mkdir build_final && cd build_final

# Configure with Visual Studio
cmake .. -G "Visual Studio 16 2019" -A x64

# Build all components
cmake --build . --config Release

# Or build specific targets
cmake --build . --config Release --target final_wav_player
```

### 3.2 Linux

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
make -j4

# Executables in current directory
```

## 4. Build Scripts Reference

### 4.1 build_final.bat (Windows)
```batch
@echo off
REM Main build script for Windows

# What it does:
# 1. Sets up Visual Studio environment
# 2. Creates build directory
# 3. Configures with CMake
# 4. Builds working components only

# Usage:
build_final.bat
```

### 4.2 build.bat (Qt GUI)
```batch
@echo off
REM Qt GUI build script

# Options:
build.bat          # Build
build.bat clean    # Clean build files
build.bat deploy   # Deploy with DLLs
```

### 4.3 run.bat (Qt GUI)
```batch
@echo off
REM Build and run in one step

# Does:
# 1. Build if needed
# 2. Deploy if built successfully
# 3. Run the application
```

## 5. Build Configuration Options

### 5.1 CMake Options

```cmake
# General options
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Windows specific
cmake .. -G "Visual Studio 16 2019" -A x64
cmake .. -G "Visual Studio 17 2022" -A x64

# Linux specific
cmake .. -DCMAKE_CXX_COMPILER=g++
cmake .. -DCMAKE_C_COMPILER=gcc

# Qt specific
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt6
```

### 5.2 Compiler Flags

#### Windows (MSVC)
```cmake
# Suppress warnings
add_compile_options(/wd4267 /wd4244)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

#### Linux (GCC/Clang)
```cmake
# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
```

## 6. Troubleshooting

### 6.1 Common Issues

#### "Qt6 not found"
```bash
# Windows
set CMAKE_PREFIX_PATH=C:\Qt\6.10.1\mingw_64

# Linux
export CMAKE_PREFIX_PATH=/usr/local/Qt-6.10.1
```

#### "No CMAKE_CXX_COMPILER could be found"
```bash
# Windows
# Install Visual Studio Build Tools or full Visual Studio

# Linux
sudo apt install build-essential  # Ubuntu/Debian
sudo dnf groupinstall "Development Tools"  # Fedora
```

#### "pthread not found" on Windows
```bash
# This is expected - Windows doesn't use pthread
# The build system handles this automatically
```

### 6.2 Debug Mode

#### Verbose Build Output
```bash
# CMake verbose mode
cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON

# Make verbose mode (Linux)
make VERBOSE=1

# MSBuild verbose mode (Windows)
cmake --build . --config Release -- /v:minimal
```

#### Clean Build
```cmd
# Windows
rd /s /q build*
del CMakeCache.txt

# Linux
rm -rf build
```

## 7. IDE Integration

### 7.1 Visual Studio Code

#### Required Extensions
- C/C++ (Microsoft)
- CMake Tools (Microsoft)
- Qt (by Jonas Tomas)

#### Configuration
```json
// .vscode/settings.json
{
    "cmake.configureEnvironment": {
        "CMAKE_PREFIX_PATH": "C:\\Qt\\6.10.1\\mingw_64"
    },
    "cmake.generator": "MinGW Makefiles"
}
```

### 7.2 Qt Creator

#### Setup
1. Open Qt Creator
2. File > Open File or Project
3. Select CMakeLists.txt
4. Configure build directory
5. Build and run

### 7.3 Visual Studio 2022

#### Setup
1. File > Open > CMake...
2. Select CMakeLists.txt
3. Wait for CMake configuration
4. Select build target
5. Build > Build Solution

## 8. Continuous Integration

### 8.1 GitHub Actions Example

```yaml
# .github/workflows/build.yml
name: Build

on: [push, pull_request]

jobs:
  windows:
    runs-on: windows-latest
    steps:
    - uses: actions/checkout@v2
    - uses: microsoft/setup-msbuild@v1
    - name: Configure
      run: cmake -B build -DCMAKE_BUILD_TYPE=Release
    - name: Build
      run: cmake --build build --config Release

  linux:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    - name: Install dependencies
      run: sudo apt update && sudo apt install -y libasound2-dev
    - name: Configure
      run: cmake -B build -DCMAKE_BUILD_TYPE=Release
    - name: Build
      run: cmake --build build
```

## 9. Distribution

### 9.1 Windows Installer

#### Using NSIS
```nsis
; installer.nsi
Name "coreMusicPlayer"
OutFile "coreMusicPlayer-Setup.exe"
InstallDir "$PROGRAMFILES\coreMusicPlayer"

Section "Main" SEC01
    SetOutPath "$INSTDIR"
    File /r "build_final\Release\*.exe"
    File /r "deploy\*.dll"
    CreateShortcut "$DESKTOP\coreMusicPlayer.lnk" "$INSTDIR\final_wav_player.exe"
SectionEnd
```

### 9.2 Linux Packages

#### Debian/Ubuntu
```bash
# Create Debian package
cpack -G DEB

# Output: coreMusicPlayer.0.0_amd64.deb
```

#### RedHat/Fedora
```bash
# Create RPM package
cpack -G RPM

# Output: coreMusicPlayer-1.0.0.x86_64.rpm
```

## 10. Performance Tips

### 10.1 Build Time Optimization

#### Using ccache (Linux)
```bash
sudo apt install ccache
export CC="ccache gcc"
export CXX="ccache g++"
```

#### Unity builds (MSVC)
```cmake
cmake .. -DCMAKE_UNITY_BUILD=ON
```

### 10.2 Build Size Optimization

#### Strip Debug Symbols
```bash
# Linux
strip final_wav_player

# Windows (after build)
build_final\Release\final_wav_player.exe
```

#### Static vs Dynamic Linking
```cmake
# Static linking (larger binary, no dependencies)
set(BUILD_SHARED_LIBS OFF)

# Dynamic linking (smaller binary, needs DLLs)
set(BUILD_SHARED_LIBS ON)
```

## 11. Performance Build Optimization

### 11.1 Compiler Optimizations

#### GCC/Clang Optimizations
```cmake
# Release optimizations with profile-guided optimization
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -mtune=native")

    # Link-time optimization
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")

    # Profile-guided optimization (two-step build)
    option(ENABLE_PGO "Enable Profile-Guided Optimization" OFF)
    if(ENABLE_PGO)
        # First build with instrumentation
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-generate")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-generate")

        # After running instrumented binary, optimize
        # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-use")
        # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-use")
    endif()
endif()
```

#### MSVC Optimizations
```cmake
if(MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        # Maximum optimization
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Ob2")

        # Enable whole program optimization
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")

        # Use link-time code generation
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /LTCG")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")

        # Favor fast code
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Oy")

        # Use parallel compilation
        set(CMAKE_CXX_FLAGS "${CXX_FLAGS} /MP")
    endif()
endif()
```

### 11.2 Build Size Optimization

#### Strip Debug Symbols
```bash
# Linux
strip final_wav_player
objcopy --strip-unneeded final_wav_player

# macOS
strip -x final_wav_player

# Windows (after build)
build_final\Release\final_wav_player.exe
```

#### Compiler-Specific Size Reduction
```cmake
# GCC/Clang
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")

# MSVC
set(CMAKE_CXX_FLAGS "${CXX_FLAGS} /Gy")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} /OPT:REF")
```

### 11.3 Dependency Management

#### Static vs Dynamic Linking
```cmake
# Static linking (larger binary, no dependencies)
option(STATIC_BUILD "Build static executable" OFF)
if(STATIC_BUILD)
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
else()
    set(BUILD_SHARED_LIBS ON)
endif()

# Bundle dependencies (Windows)
if(WIN32 AND NOT STATIC_BUILD)
    option(BUNDLE_DEPS "Bundle DLL dependencies" ON)
    if(BUNDLE_DEPS)
        include(BundleUtilities)
        bundle_app_qt()
    endif()
endif()
```

## 12. Testing the Build

### 12.1 Unit Tests
```bash
# Run all tests
ctest --output-on-failure --verbose

# Run specific test
ctest -R AudioDecoder

# Run tests with parallel execution
ctest --parallel 4
```

### 12.2 Integration Tests
```bash
# Test audio playback functionality
tests/integration/test_playback.sh

# Test plugin system
tests/integration/test_plugins.sh

# Test GUI functionality
tests/integration/test_gui.sh
```

### 12.3 Performance Tests
```bash
# Run performance benchmarks
tests/benchmarks/run_benchmarks.sh

# Memory leak detection
valgrind --tool=memcheck ./build/xpu_music
```

### 12.4 Manual Tests
```cmd
# Test WAV playback
final_wav_player.exe test_audio.wav

# Test MP3 playback (if supported)
music_player_complete.exe test_mp3.mp3

# Test Qt GUI
deploy\QtGuiPlayer.exe

# Test plugin system
music_player_complete.exe --plugin test_plugin.dll
```

## 13. Debugging the Build

### 13.1 Common Build Issues

#### Qt Not Found
```cmake
# Manual Qt configuration
set(CMAKE_PREFIX_PATH "/opt/qt6")
set(Qt6_DIR "/opt/qt6/lib/cmake/Qt6")
```

#### Library Not Found
```cmake
# Manual library configuration
set(ALSA_LIBRARY "/usr/lib/x86_64-linux-gnu/libasound.so")
set(ALSA_INCLUDE_DIR "/usr/include/alsa")
```

### 13.2 Debug Build Commands

#### Verbose Build Output
```bash
# Verbose makefile output
make VERBOSE=1

# CMake verbose configuration
cmake --debug-output --trace ..

# Ninja verbose build
ninja -v
```

### 13.3 Clean Build Procedures

#### Complete Clean
```bash
# Remove entire build directory
rm -rf build/
rm -rf build_windows/
rm -rf build_linux/
rm -f CMakeCache.txt
```

#### Incremental Clean
```cmake
# Clean and reconfigure
cmake --build . --target clean
cmake ..
```

## 14. Build Automation

### 14.1 Docker Build
```dockerfile
# Dockerfile
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libasound2-dev \
    qt6-base-dev \
    qt6-multimedia-dev \
    git \
    python3

# Copy source
COPY . /src
WORKDIR /src

# Configure and build
RUN mkdir -p build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

# Create runtime image
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y libasound2 qt6-base qt6-multimedia
COPY --from=0 /src/build/core-musicplayer /usr/local/bin/
COPY --from=0 /src/qt_gui/build/QtGuiPlayer /usr/local/bin/

CMD ["QtGuiPlayer"]
```

### 14.2 Build Matrix

#### Multi-Configuration Build Script
```bash
#!/bin/bash
# build_matrix.sh - Build multiple configurations
configs=("Debug" "Release" "RelWithDebInfo")
platforms=("Linux" "Windows" "macOS")

for config in "${configs[@]}"; do
    for platform in "${platforms[@]}"; do
        echo "Building $config for $platform..."

        case $platform in
            "Linux")
                mkdir -p build_${config}_linux
                cd build_${config}_linux
                cmake .. -DCMAKE_BUILD_TYPE=$config
                make -j$(nproc)
                ;;
            "Windows")
                mkdir -p build_${config}_windows
                cd build_${config}_windows
                cmake .. -G "Visual Studio 16 2019" -A x64 \
                      -DCMAKE_BUILD_TYPE=$config
                cmake --build . --config $config
                ;;
            "macOS")
                mkdir -p build_${config}_macos
                cd build_${config}_macos
                cmake .. -DCMAKE_BUILD_TYPE=$config
                make -j$(sysctl -n hw.ncpu)
                ;;
        esac

        cd ..
    done
done
```

## 15. Build Verification

### 15.1 Automated Verification Script
```python
#!/usr/bin/env python3
# verify_build.py - Verify build integrity
import subprocess
import sys
import os

def run_command(cmd, description):
    """Run command and return success status"""
    print(f"\n{description}...")
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"ERROR: {description} failed")
        print(f"Output: {result.stderr}")
        return False
    else:
        print(f"SUCCESS: {description}")
        return True

def verify_build():
    """Verify that build was successful"""
    checks = [
        ("./build/core-musicplayer --version", "Check main executable"),
        ("./qt_gui/build/QtGuiPlayer --version", "Check Qt GUI executable"),
        ("ctest --test-dir build", "Run unit tests"),
    ]

    all_passed = True
    for cmd, desc in checks:
        if not run_command(cmd, desc):
            all_passed = False

    if all_passed:
        print("\n✅ All build verification checks passed!")
        return 0
    else:
        print("\n❌ Some checks failed!")
        return 1

if __name__ == "__main__":
    sys.exit(verify_build())
```

This comprehensive build guide should help anyone recreate the project from scratch on any supported platform.