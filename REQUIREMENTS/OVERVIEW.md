# coreMusicPlayer System Overview

## 📖 Introduction

coreMusicPlayer is a high-performance, cross-platform audio player written in modern C++ (C++17). It provides a unified architecture that consolidates five separate player modes into a single, flexible system using the strategy pattern.

### Vision

To create a professional-grade audio player that:
- Delivers exceptional audio quality with minimal latency
- Supports extensive audio formats through an extensible plugin system
- Provides real-time audio processing capabilities
- Maintains backward compatibility with existing player modes
- Scales from basic playback to professional audio production

## 🎯 Key Features

### Audio Processing
- **Multi-format Support**: WAV, MP3, FLAC, OGG, and more through plugins
- **High-quality Resampling**: Multiple quality modes with SIMD optimization
- **Low-latency Playback**: Sub-10ms latency for real-time applications
- **Advanced DSP**: Digital signal processing with effects and filters

### Architecture
- **Unified Player Design**: Single executable with 5 operational modes
  - Legacy mode: Basic WAV playback
  - Complete mode: Full feature set
  - Realtime mode: Optimized for low latency
  - Production mode: Maximum stability
  - Multi-format mode: Extensive format support
- **Strategy Pattern**: Runtime mode switching without restart
- **Plugin System**: Foobar2000-compatible plugin architecture

### Performance
- **SIMD Optimizations**: SSE2, AVX2 support for audio processing
- **Resource Pooling**: Efficient memory management with object pools
- **Thread-safe Design**: Lock-free structures for real-time paths
- **Error Recovery**: Automatic recovery from common audio issues

## 🏗️ High-Level Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    User Interface Layer                    │
├─────────────────────┬───────────────────────────────────┤
│   Qt6 GUI          │   Command Line Interface            │
├─────────────────────┼───────────────────────────────────┤
│           Unified Player Manager (Strategy Pattern)         │
├─────────────────────┼───────────────────────────────────┤
│   Application Layer (Playlist, Metadata, Config)           │
├─────────────────────┼───────────────────────────────────┤
│            Core Audio Engine (Thread-safe)                │
├─────────────────────┼───────────────────────────────────┤
│     Decoder Manager ─── Plugin Manager ─── Feature Manager │
├─────────────────────┴───────────────────────────────────┤
│                Platform Abstraction Layer                  │
├─────────────────────┬─────────────────────┬───────────────┤
│   WASAPI (Windows) │   ALSA (Linux)      │ CoreAudio (macOS)│
└─────────────────────┴─────────────────────┴───────────────┘
```

## 🎵 Audio Processing Pipeline

```
Audio File → Decoder → Sample Rate Converter → DSP Processing → Buffer → Output
      ↓            ↓              ↓                 ↓            ↓
  File I/O    Format Parse    Resampling       Effects    Platform API
      ↓            ↓              ↓                 ↓            ↓
   Metadata    Validation     SIMD Opt.        Volume     Real-time
```

## 📊 Performance Characteristics

### Latency Targets
- **Standard Mode**: < 50ms
- **Realtime Mode**: < 10ms
- **Production Mode**: < 20ms with error recovery

### Resource Usage
- **CPU Usage**: < 5% during playback, < 1% idle
- **Memory Footprint**: < 100MB baseline, < 200MB with 100k tracks
- **Startup Time**: < 3s cold start, < 1s hot start

### Audio Quality
- **Sample Rates**: 8kHz - 192kHz
- **Bit Depths**: 16-bit, 24-bit, 32-bit float
- **Channel Config**: Mono, Stereo, Multi-channel (up to 8 channels)
- **Dynamic Range**: > 120dB with proper gain staging

## 🔧 Player Modes

### 1. Legacy Mode
- Purpose: Basic WAV playback with minimal resources
- Features:
  - WAV format only
  - Basic volume control
  - No plugins
  - 2KB buffer size
- Use Case: Embedded systems, simple playback needs

### 2. Complete Mode
- Purpose: Full-featured playback with all capabilities
- Features:
  - All supported formats
  - Full plugin system
  - Advanced DSP
  - Audio visualization
  - 4KB buffer size
- Use Case: Desktop music player with maximum features

### 3. Realtime Mode
- Purpose: Low-latency playback for live applications
- Features:
  - 64-sample buffer
  - Real-time priority
  - CPU affinity
  - Disabled plugins
  - SIMD required
- Use Case: Live performance, DJ software, audio production

### 4. Production Mode
- Purpose: Maximum reliability for critical applications
- Features:
  - Error recovery
  - Detailed logging
  - Performance monitoring
  - Optional plugins
  - 1KB buffer
- Use Case: Professional audio production, broadcast

### 5. Multi-format Mode (Default)
- Purpose: Balanced performance with extensive format support
- Features:
  - All decoders loaded
  - Intelligent format detection
  - Plugin support
  - 2KB buffer
- Use Case: General-purpose audio player

## 🎛️ Feature Management

### Core Features
- **BasicPlayback**: Fundamental audio playback capability
- **VolumeControl**: Audio volume and gain adjustment
- **SampleRateConversion**: Resampling between different rates
- **SIMDOptimization**: Hardware-accelerated processing

### Format Support
- **WAVSupport**: WAV format with various bit depths
- **MP3Support**: MP3 decoding with ID3 tags
- **FLACSupport**: Lossless audio decoding
- **OGGSupport**: OGG Vorbis decoding
- **MultiFormat**: Unified format support system

### Advanced Features
- **PluginSupport**: Extensible plugin architecture
- **RealtimeMode**: Low-latency operation mode
- **AdvancedDSP**: Digital signal processing effects
- **AudioVisualization**: Real-time audio visualization
- **GaplessPlayback**: Seamless track transitions

## 📁 File Organization

```
coreMusicPlayer/
├── src/
│   ├── core/           # Core player logic
│   │   ├── unified_music_player.cpp
│   │   ├── feature_manager.cpp
│   │   └── config_manager.cpp
│   ├── audio/          # Audio processing
│   │   ├── audio_engine.cpp
│   │   ├── decoder_manager.cpp
│   │   └── sample_rate_converter.cpp
│   ├── plugins/        # Plugin system
│   │   └── plugin_manager.cpp
│   ├── platform/       # Platform-specific code
│   │   ├── wasapi_output.cpp
│   │   └── alsa_output.cpp
│   └── gui/            # Qt6 interface
│       └── main_window.cpp
├── include/            # Public headers
├── tests/              # Unit and integration tests
├── third_party/        # External dependencies
└── cmake/              # CMake modules
```

## 🔌 Plugin Architecture

### Plugin Types
1. **Decoder Plugins**: Add support for new audio formats
2. **DSP Plugins**: Audio processing effects
3. **Output Plugins**: Custom audio output destinations
4. **Visualization Plugins**: Audio visualizations

### Foobar2000 Compatibility
coreMusicPlayer supports Foobar2000-compatible plugins through an adapter layer, providing access to a vast ecosystem of existing audio plugins.

## 🛠️ Development Stack

### Core Technologies
- **Language**: C++17
- **Build System**: CMake 3.16+
- **GUI Framework**: Qt6
- **Audio Libraries**:
  - WASAPI (Windows)
  - ALSA (Linux)
  - minimp3 (MP3 decoding)
  - FLAC (lossless audio)

### Development Tools
- **IDE**: Visual Studio 2022, CLion, Qt Creator
- **Version Control**: Git
- **CI/CD**: GitHub Actions
- **Testing**: Google Test, Catch2

## 🚀 Getting Started

### Quick Start
```bash
# Clone repository
git clone https://github.com/checkyyeug/coreMusicPlayer.git
cd coreMusicPlayer

# Configure build
mkdir build && cd build
cmake ..

# Build
cmake --build .

# Run
./coreMusicPlayer --mode multi-format
```

### Mode Selection
```bash
# Legacy mode (basic playback)
./coreMusicPlayer --mode legacy

# Complete mode (all features)
./coreMusicPlayer --mode complete

# Realtime mode (low latency)
./coreMusicPlayer --mode realtime

# Production mode (maximum stability)
./coreMusicPlayer --mode production

# Multi-format mode (default)
./coreMusicPlayer
```

## 📚 Documentation Structure

For detailed information, see:

### Requirements & Design
- [Requirements](REQUIREMENTS/PROJECT_REQUIREMENTS.md) - Functional and technical requirements
- [Architecture](DESIGN/ARCHITECTURE.md) - System architecture and design patterns
- [API Reference](DESIGN/API_DOCUMENTATION.md) - Complete API documentation

### Implementation
- [Audio Engine](IMPLEMENTATION/AUDIO_ENGINE.md) - Audio processing implementation
- [Platform Specifics](IMPLEMENTATION/PLATFORM_SPECIFICS.md) - Platform-specific code
- [Source Examples](IMPLEMENTATION/SOURCE_EXAMPLES.md) - Code examples and patterns

### Build & Deploy
- [Build Guide](BUILD_AND_DEPLOY/BUILD_GUIDE.md) - Build instructions
- [Dependencies](BUILD_AND_DEPLOY/DEPENDENCIES.md) - Dependencies and licensing
- [Deployment](BUILD_AND_DEPLOY/DEPLOYMENT.md) - Distribution and deployment

### Support
- [Troubleshooting](SUPPORT/TROUBLESHOOTING.md) - Common issues and solutions
- [FAQ](SUPPORT/FAQ.md) - Frequently asked questions
- [Changelog](SUPPORT/CHANGELOG.md) - Version history

## 🤝 Contributing

We welcome contributions! Please see our contributing guidelines in the repository.

### Key Areas for Contribution
1. **Audio Format Support**: Add new decoder plugins
2. **Platform Support**: Help with new platforms
3. **Performance**: Optimize critical paths
4. **Documentation**: Improve and extend documentation
5. **Testing**: Add comprehensive tests

## 📄 License

coreMusicPlayer is licensed under the GPL-3.0 license. See [LICENSE](LICENSE) for details.

---

*This overview provides a high-level understanding of coreMusicPlayer. For detailed technical information, please refer to the specific documentation sections.*