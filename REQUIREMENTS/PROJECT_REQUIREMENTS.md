# coreMusicPlayer - Project Requirements

## 1. Project Overview

coreMusicPlayer is a cross-platform audio player application designed with extensibility in mind. It supports multiple audio formats, provides high-quality audio processing, and integrates with Foobar2000 SDK for plugin compatibility.

### 1.1 Project Scope
The project consists of multiple audio player implementations, each serving different purposes:
- **music_player_legacy**: Basic WAV player with minimal dependencies
- **music_player_complete**: Full-featured player with plugin support
- **real_player**: Real-time audio processing player
- **final_wav_player**: Production-ready WAV player
- **multi_format_player**: Main application with multi-format support

### 1.2 Key Design Principles
- **Modular Architecture**: Separate core library (xpumusic_lib) from executables
- **Platform Abstraction**: Unified API with platform-specific implementations
- **Plugin Extensibility**: Support for Foobar2000-compatible plugins
- **Performance First**: Optimized for low-latency real-time playback

## 2. Core Requirements

### 2.1 Functional Requirements

#### 2.1.1 Audio Playback
- **FR-001**: Support for WAV, MP3, FLAC, OGG audio formats
  - WAV: Native PCM support (8, 16, 24, 32-bit)
  - MP3: Through minimp3 library or Foobar2000 plugins
  - FLAC/OGG: Via Foobar2000 decoder plugins
- **FR-002**: High-quality audio resampling with multiple algorithms
  - Linear interpolation (fast)
  - Cubic interpolation (good)
  - Sinc interpolation (best)
  - Pre-computed tables (high quality)
- **FR-003**: Real-time audio playback with low latency (< 50ms)
  - Windows: WASAPI exclusive mode support
  - Linux: ALSA low-latency configuration
- **FR-004**: Basic playback controls
  - Load audio file with format validation
  - Play/Pause/Stop functionality
  - Seek to percentage position
  - Real-time status display
- **FR-005**: Support for common sample rates
  - 44.1kHz, 48kHz, 96kHz, 192kHz
  - Automatic sample rate conversion when needed

#### 2.1.2 Audio Processing
- **FR-011**: Sample rate conversion with quality modes
- **FR-012**: Volume control with gain adjustment
- **FR-013**: Basic channel handling (stereo/mono)
- **FR-014**: Multiple bit-depth support (16, 24, 32-bit)
- **FR-015**: SIMD optimization for performance
  - SSE2 for basic vector operations
  - AVX/AVX2 for advanced processing
  - Fallback to scalar implementation

#### 2.1.3 User Interface
- **FR-019**: Command-line interface (CLI) for all players
  - Interactive mode with command prompt
  - Direct playback mode: `player <file>`
  - Command set: load, play, pause, stop, seek, status, help, quit
- **FR-020**: Qt6-based GUI (gui/qt_gui)
  - Basic playback controls
  - File drag-and-drop support
  - Real-time visualization (optional)
- **FR-021**: Configuration management
  - INI-based configuration file
  - Runtime parameter adjustment
  - Persistent settings storage

#### 2.1.4 Foobar2000 Integration
- **FR-027**: Compatibility with Foobar2000 audio decoder plugins
  - Plugin loading via FoobarPluginManager
  - Dynamic plugin discovery and initialization
- **FR-028**: Support for Foobar2000 DSP plugins
  - Real-time audio processing pipeline
  - Parameter configuration and persistence
- **FR-029**: Plugin management system
  - Enable/disable plugins at runtime
  - Plugin parameter storage in config
- **FR-030**: Basic plugin configuration interface

### 2.2 Non-Functional Requirements

#### 2.2.1 Performance
- **NFR-001**: Startup time < 3 seconds (cold), < 1 second (warm)
- **NFR-002**: CPU usage < 5% during playback, < 1% when idle
- **NFR-003**: Memory usage < 100MB (baseline), < 200MB (with 100k tracks)
- **NFR-004**: Support for playlists up to 100,000 tracks
- **NFR-005**: Audio latency < 10ms (exclusive mode), < 20ms (shared)
- **NFR-006**: Track switching time < 500ms (local), < 2s (network)
- **NFR-007**: Seek operation response time < 50ms
- **NFR-008**: Gapless playback transition < 10ms

#### 2.2.2 Compatibility
- **NFR-005**: Windows 10/11 (64-bit) primary support
- **NFR-006**: Linux (Ubuntu 20.04+) secondary support
- **NFR-007**: macOS 11+ future support (planned)
- **NFR-008**: Backward compatibility with Windows 7 (if possible)

#### 2.2.3 Reliability
- **NFR-009**: Zero crashes during normal operation
- **NFR-010**: Graceful error handling for corrupt files
- **NFR-011**: Automatic recovery from audio device failures
- **NFR-012**: Memory leak free operation

#### 2.2.4 Extensibility
- **NFR-013**: Plugin architecture for custom audio processors
- **NFR-014**: Theme support through CSS-like stylesheets
- **NFR-015**: Scriptable automation interface
- **NFR-016**: HTTP API for remote control

## 3. Implementation Architecture

### 3.1 Core Components

#### 3.1.1 Audio Players Implementation
```
coreMusicPlayer/
├── music_player_legacy.cpp      # Basic WAV player
├── music_player_complete.cpp    # Full-featured player
├── real_player.cpp              # Real-time processing
├── final_wav_player.cpp         # Production WAV player
└── multi_format_player.cpp      # Main application
```

#### 3.1.2 Core Library Structure
```
src/
├── core/
│   ├── music_player.h/cpp       # Base player interface
│   ├── config_manager.h/cpp     # Configuration management
│   └── foobar_plugin_manager.h/cpp # Plugin system
├── audio/
│   ├── wav_parser.h/cpp         # WAV file handling
│   ├── sample_rate_converter.h/cpp # Resampling
│   └── simd_optimizations.h/cpp # Vector operations
└── platform/
    ├── windows/
    │   └── wasapi_output.h/cpp   # Windows audio
    └── linux/
        └── alsa_output.h/cpp    # Linux audio
```

### 3.2 Platform-Specific Implementations

#### 3.2.1 Windows (WASAPI)
```cpp
// Key Windows implementation details
class WindowsAudioOutput {
private:
    IAudioClient* audio_client_;
    IAudioRenderClient* render_client_;
    WAVEFORMATEX* mix_format_;

public:
    bool initialize(const AudioFormat& format);
    void write(const float* buffer, size_t frames);
    int getLatency() const;
};
```

#### 3.2.2 Linux (ALSA)
```cpp
// Key Linux implementation details
class LinuxAudioOutput {
private:
    snd_pcm_t* pcm_handle_;
    snd_pcm_hw_params_t* hw_params_;

public:
    bool initialize(const AudioFormat& format);
    void write(const float* buffer, size_t frames);
    int getLatency() const;
};
```

## 4. Technical Requirements

### 4.1 Programming Language and Frameworks
- **Primary Language**: C++17
- **GUI Framework**: Qt6 (optional, for gui/qt_gui)
- **Build System**: CMake 3.10+
- **Platform APIs**:
  - Windows: WASAPI (Windows Audio Session API)
  - Linux: ALSA (Advanced Linux Sound Architecture)
  - macOS: CoreAudio (planned for future)

### 4.2 External Dependencies
- **Core Dependencies**:
  - Standard C++17 libraries
  - Windows SDK (Windows)
  - ALSA development libraries (Linux)
- **Optional Dependencies**:
  - Foobar2000 SDK (plugin compatibility)
  - Qt6 (GUI components)
  - minimp3 (MP3 decoding)
  - nlohmann/json (configuration)
- **Build Tools**:
  - Visual Studio 2019+ (Windows)
  - GCC 9+ / Clang 10+ (Linux)
  - CMake 3.10+
- **Optional Libraries**:
  - nlohmann/json (for configuration)
  - FFTW (for advanced audio processing)

### 3.3 File Format Support

#### 3.3.1 WAV (WAVE)
- Uncompressed PCM
- IEEE float (32-bit)
- Multiple sample rates
- Multi-channel support (up to 7.1)

#### 3.3.2 MP3
- MPEG-1/2/2.5 Layer III
- VBR, CBR, ABR
- ID3v1/v2 tags
- 8-320 kbps

#### 3.3.3 FLAC
- Free Lossless Audio Codec
- 16-24 bit depth
- Metadata support

#### 3.3.4 OGG Vorbis
- Variable bitrate
- Metadata tags
- Channel mapping

## 4. User Stories

### 4.1 Basic User
- As a music listener, I want to play my local music files easily
- As a music listener, I want to create and manage playlists
- As a music listener, I want to adjust volume and seek in tracks

### 4.2 Advanced User
- As an audiophile, I want high-quality audio processing
- As an audiophile, I want support for high-resolution audio formats
- As a power user, I want to use Foobar2000 plugins

### 4.3 Developer
- As a developer, I want to extend the player with custom plugins
- As a developer, I want a clean, well-documented API
- As a developer, I want cross-platform compatibility

## 5. Constraints and Assumptions

### 5.1 Constraints
- Must not depend on proprietary codecs
- Must maintain compatibility with existing Foobar2000 plugins
- Windows is the primary target platform
- Must work without internet connection

### 5.2 Assumptions
- Users have modern hardware (multi-core CPU, SSE4+)
- Audio drivers are properly installed
- Sufficient disk space for audio files
- Basic computer literacy

## 6. Success Criteria

### 6.1 Must-Have
- Play common audio formats without errors
- Stable operation on Windows 10
- Basic GUI functionality
- Plugin system works with test plugins

### 6.2 Should-Have
- Support for high-resolution audio
- Linux compatibility
- Performance optimizations
- Advanced audio features

### 6.3 Could-Have
- Visualizations
- Web interface
- Streaming support
- Mobile app

## 7. Risk Assessment

### 7.1 Technical Risks
- **High**: Foobar2000 SDK complexity
- **Medium**: Cross-platform audio API differences
- **Low**: Qt framework learning curve

### 7.2 Mitigation Strategies
- Start with Windows-only implementation
- Create abstraction layers for platform differences
- Use proven audio libraries
- Incremental development approach

## 8. Technical Implementation Details

### 8.1 Audio Buffer Management

```cpp
// Audio buffer with SIMD alignment
class AudioBuffer {
private:
    static constexpr size_t ALIGNMENT = 32; // AVX requires 32-byte alignment
    size_t frames_;
    int channels_;
    std::unique_ptr<float[]> data_;

public:
    AudioBuffer(int channels, size_t frames)
        : frames_(frames), channels_(channels) {
        // Allocate aligned memory
        data_ = std::make_unique<float[]>(frames * channels);
    }

    // SIMD operations
    void applyGain(float gain) {
#ifdef __AVX2__
        __m256 gain_vec = _mm256_set1_ps(gain);
        for (size_t i = 0; i < frames_ * channels_; i += 8) {
            __m256 samples = _mm256_load_ps(data_.get() + i);
            samples = _mm256_mul_ps(samples, gain_vec);
            _mm256_store_ps(data_.get() + i, samples);
        }
#else
        for (size_t i = 0; i < frames_ * channels_; ++i) {
            data_[i] *= gain;
        }
#endif
    }
};
```

### 8.2 Platform Abstraction Example

```cpp
// Cross-platform audio output interface
class AudioOutput {
public:
    virtual bool initialize(const AudioFormat& format) = 0;
    virtual void write(const float* buffer, size_t frames) = 0;
    virtual void cleanup() = 0;
};

// Windows implementation
class WASAPIOutput : public AudioOutput {
    IAudioClient* audio_client_;
    IAudioRenderClient* render_client_;

public:
    bool initialize(const AudioFormat& format) override {
        // WASAPI initialization code
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        // ... device enumeration and setup
        return SUCCEEDED(hr);
    }
};

// Linux implementation
class ALSAOutput : public AudioOutput {
    snd_pcm_t* pcm_handle_;

public:
    bool initialize(const AudioFormat& format) override {
        // ALSA initialization code
        int err = snd_pcm_open(&pcm_handle_, "default",
                               SND_PCM_STREAM_PLAYBACK, 0);
        // ... hardware parameter setup
        return err == 0;
    }
};
```

### 8.3 Sample Rate Conversion Algorithms

```cpp
// Sample rate converter with multiple quality modes
class SampleRateConverter {
public:
    enum Quality {
        Fast,    // Linear interpolation
        Good,    // Cubic interpolation
        Best,    // Sinc interpolation
        High     // High-quality with pre-computed tables
    };

    SampleRateConverter(int inputRate, int outputRate, Quality quality)
        : quality_(quality) {
        // Initialize based on quality
        switch(quality) {
            case Fast:
                useLinearInterpolation();
                break;
            case Good:
                useCubicInterpolation();
                break;
            case Best:
                useSincInterpolation();
                break;
            case High:
                useHighQualitySincWithTables();
                break;
        }
    }

    size_t process(const float* input, float* output, size_t frames) {
        // Process audio based on selected algorithm
        switch(quality_) {
            case Fast:
                return linearResample(input, output, frames);
            // ... other cases
        }
    }
};
```

### 8.4 Plugin System Architecture

```cpp
// Plugin manager for Foobar2000 compatibility
class PluginManager {
private:
    std::map<std::string, HMODULE> loaded_plugins_;
    std::vector<PluginInterface*> active_plugins_;

public:
    bool loadPlugin(const std::string& path) {
        // Load DLL
        HMODULE handle = LoadLibraryA(path.c_str());
        if (!handle) return false;

        // Get plugin entry point
        typedef PluginInterface* (*GetPluginFunc)();
        GetPluginFunc getPlugin = (GetPluginFunc)GetProcAddress(
            handle, "getPlugin");

        if (!getPlugin) {
            FreeLibrary(handle);
            return false;
        }

        // Initialize plugin
        PluginInterface* plugin = getPlugin();
        if (plugin && plugin->initialize()) {
            loaded_plugins_[path] = handle;
            active_plugins_.push_back(plugin);
            return true;
        }

        return false;
    }

    void processAudio(float* buffer, size_t frames) {
        for (auto* plugin : active_plugins_) {
            if (plugin->isEnabled()) {
                plugin->process(buffer, frames);
            }
        }
    }
};
```

## 9. Project Phases

### Phase 1: Core Audio Engine (4 weeks)
- Basic WASAPI implementation
- WAV and MP3 decoding
- Simple CLI interface

### Phase 2: GUI Development (3 weeks)
- Qt6 GUI implementation
- Playlist management
- Basic controls

### Phase 3: Advanced Features (4 weeks)
- Sample rate conversion
- Plugin system
- Performance optimizations

### Phase 4: Linux Support (3 weeks)
- ALSA implementation
- Platform abstraction
- Testing and debugging

### Phase 5: Polish and Release (2 weeks)
- Bug fixes
- Documentation
- Installer creation

## 10. Quality Metrics

### 10.1 Performance Benchmarks
- **Audio Throughput**: Must handle 192kHz/32-bit stereo without drops
- **Memory Efficiency**: < 1MB per minute of audio loaded
- **CPU Efficiency**: < 0.5% per channel of real-time processing
- **Startup Time**: < 2 seconds on SSD, < 5 seconds on HDD

### 10.2 Reliability Metrics
- **MTBF**: > 1000 hours of continuous playback
- **Error Rate**: < 0.001% audio glitches under normal load
- **Recovery Time**: < 1 second from audio device failure

### 10.3 Usability Metrics
- **Learning Curve**: Basic functions usable within 5 minutes
- **Task Completion**: Common tasks < 3 clicks
- **Error Recovery**: All errors recoverable without restart