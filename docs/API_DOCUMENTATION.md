# coreMusicPlayer API Documentation

## Overview

coreMusicPlayer is a high-performance cross-platform audio player with support for Foobar2000 plugin integration.

## Core Modules

### 1. Core Module
The core module contains fundamental classes and utilities used throughout the application.

#### Classes
- **UnifiedMusicPlayer**: Main player class that manages different playback strategies
- **StrategyFactory**: Factory for creating different player strategies
- **Result<T>**: Template class for error handling
- **Error**: Error handling class

### 2. Audio Module
The audio module handles all audio processing and playback functionality.

#### Classes
- **AudioEngine**: Main audio engine interface and implementation
- **DeviceManager**: Manages audio devices
- **SampleRateConverter**: Handles audio sample rate conversion
- **VolumeControl**: Digital volume control
- **Equalizer**: Multi-band equalizer

### 3. Foobar2000 Integration Module
The foobar module provides integration with the Foobar2000 plugin system.

#### Classes
- **FoobarPluginManager**: Manages Foobar2000 plugins
- **FoobarInputAdapter**: Adapts Foobar2000 input plugins
- **FoobarDSPAdapter**: Adapts Foobar2000 DSP plugins
- **FoobarOutputAdapter**: Adapts Foobar2000 output plugins

### 4. GUI Module
The GUI module provides the user interface components.

#### Classes
- **MainWindow**: Main application window
- **ThemeManager**: Manages UI themes

## Key Features

### Playback Strategies
coreMusicPlayer supports multiple playback strategies:
1. **Legacy Strategy**: Basic playback with minimal dependencies
2. **Complete Strategy**: Full feature set with all audio processing
3. **Realtime Strategy**: Low-latency real-time audio processing
4. **Production Strategy**: Production-ready with error handling
5. **MultiFormat Strategy**: Automatic format detection and support

### Audio Processing
- Support for multiple audio formats (WAV, MP3, FLAC, OGG)
- Digital volume control with linear/logarithmic scaling
- Multi-band equalizer with adjustable bands
- DSP effects processing
- SIMD optimizations for performance

### Foobar2000 Integration
- Plugin management system
- Input plugin adapter
- DSP processor adapter
- Output plugin adapter

## Usage Example

```cpp
#include "core_music_player.h"
#include "core/unified_music_player.h"

// Initialize the player
auto player = core::UnifiedMusicPlayer::instance();
player->initialize();

// Set playback strategy
player->set_strategy("complete");

// Load and play audio file
player->load_file("example.mp3");
player->play();
```

## Build Instructions

### Windows (Visual Studio)
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Linux/macOS (GCC/Clang)
```bash
mkdir build
cd build
cmake ..
make
```