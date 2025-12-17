# coreMusicPlayer File Structure Documentation

## 1. Root Directory Structure

```
coreMusicPlayer/
│
├── src/                          # Core source files
│   ├── audio/                    # Audio processing modules
│   │   ├── sample_rate_converter.cpp
│   │   ├── sample_rate_converter.h
│   │   ├── simd_optimizations.cpp
│   │   └── simd_optimizations.h
│   ├── foobar_plugin_manager.cpp  # Foobar2000 plugin integration
│   ├── foobar_plugin_manager.h
│   ├── simple_wav_player.cpp     # Simple WAV playback
│   ├── simple_wav_player.h
│   ├── test_wasapi.cpp           # WASAPI testing
│   └── network_audio_player.cpp  # Network streaming
│
├── qt_gui/                       # Qt6 GUI implementation
│   ├── CMakeLists.txt           # Qt GUI build config
│   ├── main.cpp                 # GUI application entry
│   ├── mainwindow.cpp/h         # Main window implementation
│   ├── playerwidget.cpp/h       # Player control widget
│   ├── playlistmodel.cpp/h      # Playlist data model
│   ├── build.bat                # Build script
│   ├── run.bat                  # Build and run script
│   └── test.bat                 # Quick test script
│
├── gui/                          # Alternative GUI implementation
│   ├── CMakeLists.txt           # Build configuration
│   ├── mainwindow.cpp/h         # Main window
│   ├── playerwidget.cpp/h       # Player widget
│   └── playlistmodel.cpp/h      # Playlist model
│
├── config/                       # Configuration management
│   ├── config_manager.cpp
│   └── config_manager.h
│
├── core/                         # Core engine (planned)
│   ├── core_engine.cpp/h
│   ├── service_registry.cpp/h
│   ├── event_bus.cpp/h
│   ├── plugin_host.cpp/h
│   ├── config_manager.cpp/h
│   ├── playlist_manager.cpp/h
│   ├── playback_engine.cpp/h
│   └── visualization_engine.cpp/h
│
├── platform/                     # Platform abstraction
│   ├── audio_output_factory.cpp/h
│   └── linux/
│       └── audio_output_stub.cpp
│
├── compat/                       # Compatibility layers
│   ├── sdk_implementations/
│   │   ├── audio_decoder.h
│   │   ├── audio_output.h
│   │   └── simple_dsp.h
│   ├── foobar_sdk/
│   └── xpumusic_sdk/
│
├── docs/                         # Documentation (moved from root)
│   ├── DEPENDENCIES.md
│   ├── ENGINEERING_SUMMARY.md
│   └── README.md
│
├── third_party/                  # Third-party libraries
│   └── nlohmann-json/            # JSON library (header-only)
│
├── test_audio/                   # Test audio files
├── build_* directories         # Build output directories
├── CMakeLists.txt               # Main CMake configuration
├── CMakeLists_simplified.txt    # Simplified CMake config
├── build_final.bat              # Main Windows build script
├── README.md                    # Project README
└── Various executables (cpp)    # Command-line tools
```

## 2. Core Source Files Details

### 2.1 Audio Processing (`src/audio/`)

#### sample_rate_converter.cpp/h
```cpp
class SampleRateConverter {
public:
    enum Quality { Fast, Good, Best, High };

    bool resample(const AudioBuffer& input, AudioBuffer& output,
                  double inputRate, double outputRate,
                  Quality quality = Quality::Good);

private:
    // Implementation uses:
    // - Linear interpolation (Fast)
    // - Cubic spline (Good)
    // - Kaiser-windowed sinc (Best/High)
    // - SIMD optimizations for x86/x64
};
```

#### simd_optimizations.cpp/h
```cpp
// SIMD-optimized audio processing functions
namespace simd {
    void adjust_volume_avx(float* data, size_t frames, float volume);
    void adjust_volume_sse2(float* data, size_t frames, float volume);
    void resample_cubic_sse2(const float* input, float* output, ...);
    void convert_format_simd(const void* input, void* output, ...);
}
```

### 2.2 Plugin Integration

#### foobar_plugin_manager.cpp/h
```cpp
class PluginManager {
public:
    bool loadPlugin(const std::string& pluginPath);
    void unloadPlugin(const std::string& pluginName);
    std::vector<PluginInfo> getLoadedPlugins();

    // Service registry integration
    template<typename T>
    T* getService(const char* name);
};
```

### 2.3 Audio Players

#### simple_wav_player.cpp/h
- Basic WAV file player
- Platform-specific audio output
- Simple command-line interface

#### test_wasapi.cpp
- WASAPI testing and diagnostics
- Audio device enumeration
- Latency measurement

## 3. GUI Implementation Details

### 3.1 Qt6 GUI (`qt_gui/`)

#### mainwindow.cpp/h
```cpp
class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    PlayerWidget* m_playerWidget;
    QListView* m_playlistView;
    QMediaPlayer* m_mediaPlayer;
    QMediaPlaylist* m_playlist;

    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void connectSignals();
};
```

#### playerwidget.cpp/h
```cpp
class PlayerWidget : public QWidget {
    Q_OBJECT
signals:
    void playPauseRequested();
    void stopRequested();
    void nextTrackRequested();
    void previousTrackRequested();
    void volumeChanged(int volume);

private slots:
    void onPlayPauseClicked();
    void onStopClicked();
    void onPositionSliderMoved(int position);
};
```

### 3.2 Build Scripts

#### build.bat
```batch
@echo off
# Main build script with options:
#   build.bat         - Build
#   build.bat clean   - Clean
#   build.bat deploy  - Deploy DLLs
```

## 4. Configuration System (`config/`)

#### config_manager.cpp/h
```cpp
class ConfigManager {
public:
    template<typename T>
    T get(const std::string& key, const T& defaultValue = T{});

    template<typename T>
    void set(const std::string& key, const T& value);

    void load();
    void save();

private:
    nlohmann::json config_;
    std::string configFile_;
};
```

## 5. Executable Files

### 5.1 Console Applications

| Executable | Purpose | Dependencies |
|------------|---------|--------------|
| `final_wav_player.cpp` | Main WAV player | WASAPI |
| `music_player_complete.cpp` | Full-featured player | Qt6 (optional) |
| `music_player_legacy.cpp` | Legacy player | WASAPI |
| `music_player_simple_direct.cpp` | Direct WASAPI player | WASAPI |
| `simple_player.cpp` | Simple CLI player | Basic I/O |
| `multi_format_player.cpp` | Multi-format support | Various decoders |

### 5.2 Testing and Utilities

| Executable | Purpose |
|------------|---------|
| `test_simple_playback.cpp` | Playback testing |
| `simple_performance_test.cpp` | Performance benchmarks |
| `optimization_integration_example.cpp` | SIMD optimization demo |
| `check_config.cpp` | Configuration testing |
| `test_plugin_config.cpp` | Plugin configuration test |

## 6. Build Directories

### 6.1 Generated Build Directories
```
build/            # Original build (88MB)
build_clean/       # Clean build attempt
build_final/       # Final working build (5.4MB)
build_fixed/       # Fixed version build (574KB)
build_working/     # Working components build
```

### 6.2 Build Output Structure
```
build_final/
├── Release/
│   ├── final_wav_player.exe
│   ├── music_player_complete.exe
│   ├── music_player_legacy.exe
│   ├── music_player_simple_direct.exe
│   ├── optimization_integration_example.exe
│   ├── simple_performance_test.exe
│   └── test_simple_playback.exe
└── CMakeFiles/    # CMake generated files
```

## 7. Third-Party Dependencies

### 7.1 nlohmann/json (`third_party/nlohmann-json/`)
- Header-only JSON library
- Used for configuration file format
- Version: 3.11.2 (included)

### 7.2 Foobar2000 SDK
- External SDK (not included)
- Path: `foobar2000_sdk/`
- Used for plugin compatibility

## 8. Platform-Specific Files

### 8.1 Windows
- WASAPI implementation files
- Windows-specific build scripts
- Visual Studio project files

### 8.2 Linux
- ALSA implementation files
- Platform abstraction stubs
- Linux-specific makefiles

## 9. Documentation Files

### 9.1 Root Documentation
```
README.md                    # Project overview
WARNING_FIXES.md            # Build warning fixes
BUILD_GUIDE.md              # Build instructions (now in docs/)
```

### 9.2 Moved Documentation (`docs/`)
```
docs/
├── DEPENDENCIES.md          # External dependencies
├── ENGINEERING_SUMMARY.md   # Project summary
├── FOOBAR2000_COMPATIBILITY_ANALYSIS.md
├── MUSIC_PLAYER_SUCCESS.md
├── PERFORMANCE_OPTIMIZATION_SUMMARY.md
└── README.md
```

## 10. Configuration Files

### 10.1 CMake Configuration
```
CMakeLists.txt               # Main CMake file (simplified)
CMakeLists_simplified.txt    # Backup simplified version
qt_gui/CMakeLists.txt        # Qt GUI CMake
gui/CMakeLists.txt           # Alternative GUI CMake
```

### 10.2 Build Scripts
```
build_final.bat              # Main build script
build_windows.bat            # Windows-specific script
build_gui_all.bat            # GUI build script
cleanup_build_dirs.bat       # Cleanup utility
```

## 11. Test Assets

### 11.1 Audio Files
```
test_audio.wav               # Basic test audio
test_1khz.wav               # 1kHz test tone
test_stereo.wav              # Stereo test file
simple_test.wav             # Simple test
```

### 11.2 Generated Test Audio (`src/audio/`)
Various test files at different sample rates and formats for testing resampling and format conversion.

## 12. Implementation Checklist

For recreating this project from scratch:

1. **Set up build environment**
   - Install CMake 3.10+
   - Install Visual Studio 2019+ (Windows) or GCC 9+ (Linux)
   - Install Qt6 (for GUI)

2. **Create directory structure**
   ```
   mkdir -p src/{audio,core,platform,compat}
   mkdir -p {qt_gui,gui,config}
   mkdir -p docs third_party
   ```

3. **Core files to create**
   - `CMakeLists.txt` (use simplified version)
   - Audio processing files in `src/audio/`
   - GUI files in `qt_gui/`
   - Configuration system

4. **Build and test**
   - Run `build_final.bat` (Windows) or make (Linux)
   - Test with provided audio files
   - Verify all executables work

This file structure provides a complete blueprint for recreating the entire coreMusicPlayer project with all its features and components.